
#ifndef GPR924_ENGINE_INPUT_MANAGER_H
#define GPR924_ENGINE_INPUT_MANAGER_H

/*
Copyright 2026 SAE Institute Switzerland SA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Contributors: Elias Farhan
*/


#include <cstdint>
#include <expected>
#include <span>
#include <stdexcept>
#include <vector>
#include <array>

#include "container/small_vector.h"

namespace common {

// Strongly-typed wrapper for a player index.
class PlayerNumber {
 public:
  explicit constexpr PlayerNumber(int index) : index_(index) {}

  [[nodiscard]] constexpr size_t index() const {
    return static_cast<size_t>(index_);
  }
  [[nodiscard]] constexpr int signed_index() const { return index_; }

 private:
  int index_;
};

// Strongly-typed wrapper for a frame number in the simulation timeline.
class Frame {
 public:
  explicit constexpr Frame(int index) : index_(index) {}
  explicit constexpr Frame(size_t index) : index_(static_cast<int>(index)) {}

  [[nodiscard]] constexpr size_t index() const {
    return static_cast<size_t>(index_);
  }
  [[nodiscard]] constexpr int signed_index() const { return index_; }

 private:
  int index_;
};

// Manages per-player input history for rollback netcode, supporting speculative
// inputs and network synchronization.
template <typename InputT, int kMaxInputHistory, int kMaxPlayerCount>
class InputManager {
 public:
  InputManager() { inputs_.resize(kMaxInputHistory); }
  // Returns the input for the given player at the given frame. Falls back to
  // the latest stored input if the frame is beyond history.
  [[nodiscard]] InputT input(PlayerNumber player_number,
                             Frame current_frame) const {
    const auto frame_index = index(current_frame);
    if (frame_index >= inputs_.size()) {
      return inputs_.back()[player_number.index()];
    }
    return inputs_[frame_index][player_number.index()];
  }

  // Sets the input for a player at a specific frame. If this is the latest
  // received frame for that player, propagates the input forward as speculative
  // input for all subsequent stored frames.
  void set_input(PlayerNumber player_number, InputT input,
                 Frame current_frame) {
    auto& input_metadata = input_metadata_[player_number.index()];
    bool is_latest_input_receive = false;
    if (input_metadata.last_received_frame.signed_index() <
        current_frame.signed_index()) {
      input_metadata.last_received_frame = current_frame;
      is_latest_input_receive = true;
    }

    auto frame_index = index(current_frame);
    if (frame_index >= kMaxInputHistory) {
      throw std::runtime_error("Over-capacity of the input history");
    }
    if (frame_index >= inputs_.size()) {
      inputs_.resize(
          kMaxInputHistory,
          inputs_.back());  // no allocation as we pre-allocated max history
    }
    inputs_[frame_index][player_number.index()] = input;
    if (is_latest_input_receive) {
      // speculative inputs, we just copy the input further
      for (; frame_index < inputs_.size(); ++frame_index) {
        inputs_[frame_index][player_number.index()] = input;
      }
    }
  }

  // Returns a vector of all inputs for a player from the first stored frame up
  // to and including the given frame.
  [[nodiscard]] core::SmallVector<InputT, kMaxInputHistory> inputs(
      PlayerNumber player_number, Frame current_frame) const {
    core::SmallVector<InputT, kMaxInputHistory> result;
    const auto window_size = index(current_frame) + 1;
    if (window_size > kMaxInputHistory) {
      throw std::runtime_error("We cannot get more inputs that we store");
    }
    for (size_t i = 0; i < window_size; ++i) {
      result.push_back(inputs_[i][player_number.index()]);
    }
    return result;
  }

  // Returns a span of all player inputs for a single frame.
  [[nodiscard]] std::span<const InputT, kMaxPlayerCount> inputs(
      Frame current_frame) const {
    return inputs_[index(current_frame)];
  }

  [[nodiscard]] Frame last_confirm_frame() const { return last_confirm_frame_; }
  [[nodiscard]] Frame last_received_frame() const {
    auto smaller_frame = Frame(-1);
    for (size_t player_index = 0;
         player_index < static_cast<size_t>(kMaxPlayerCount); ++player_index) {
      if (input_metadata_[player_index].is_valid &&
          smaller_frame.index() >
              input_metadata_[player_index].last_received_frame.index()) {
        smaller_frame = input_metadata_[player_index].last_received_frame;
      }
    }
    return smaller_frame;
  }
  [[nodiscard]] Frame last_received_frame(PlayerNumber player_number) const {
    return input_metadata_[player_number.index()].last_received_frame;
  }

  // Applies a batch of inputs received from the network for a player. Marks
  // the manager as dirty if any input differs from the previously stored value,
  // and updates speculative inputs for frames beyond the received window.
  void set_inputs_from_network(PlayerNumber player_number,
                               std::span<InputT> inputs, Frame current_frame) {
    const auto window_size = inputs.size();
    const auto first_frame = current_frame.index() - window_size + 1;



    for (size_t offset = 0; offset < window_size; ++offset) {
      const auto frame = Frame{first_frame + offset};
      const auto last_input_value = input(player_number, frame);
      if (last_input_value != inputs[offset]) {
        is_dirty_ = true;
      }
      set_input(player_number, inputs[offset], frame);
    }
    // Actual speculative input
    if (current_frame.signed_index() > input_metadata_[player_number.index()]
                                   .last_received_frame.signed_index()) {
      const auto frame_index = index(current_frame);
      for (auto i = frame_index; i < inputs_.size(); ++i) {
        inputs_[i][player_number.index()] = inputs.back();
      }
    }
  }

  [[nodiscard]] bool is_valid(PlayerNumber player_number) const {
    return input_metadata_[player_number.index()].is_valid;
  }
  void set_valid(PlayerNumber player_number, bool valid) {
    input_metadata_[player_number.index()].is_valid = valid;
  }
  [[nodiscard]] bool is_dirty() const { return is_dirty_; }
// Resets the dirty flag after a rollback-and-resimulate has been performed.
void CleanDirty() { is_dirty_ = false; }

  // Advances the confirmed frame by one, discarding the oldest stored frame.
  void ConfirmFrame() {
    inputs_.erase(inputs_.begin());
    last_confirm_frame_ = Frame{last_confirm_frame_.signed_index() + 1};
  }

 private:
  struct InputMetadata {
    Frame last_received_frame = Frame(-1);
    bool is_valid = false;
  };

  [[nodiscard]] size_t index(Frame current_frame) const {
    const auto last_confirm_frame_offset =
        static_cast<size_t>(last_confirm_frame_.signed_index() + 1);
    return current_frame.index() - last_confirm_frame_offset;
  }
  std::vector<std::array<InputT, kMaxPlayerCount>> inputs_;
  std::array<InputMetadata, kMaxPlayerCount> input_metadata_{};
  Frame last_confirm_frame_{-1};
  bool is_dirty_ = false;
};

}  // namespace common

#endif  // GPR924_ENGINE_INPUT_MANAGER_H
