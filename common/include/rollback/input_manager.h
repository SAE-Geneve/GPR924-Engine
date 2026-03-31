
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

#include <expected>
#include <span>
#include <stdexcept>
#include <vector>
#include <cstdint>

namespace common {

class PlayerNumber {
 public:
  explicit constexpr PlayerNumber(int index):index_(index){}

  [[nodiscard]] constexpr size_t index() const { return static_cast<size_t>(index_); }
  [[nodiscard]] constexpr int signed_index() const { return index_; }

 private:
  int index_;
};

class Frame {
 public:
  explicit constexpr  Frame(int index):index_(index){}

  [[nodiscard]] constexpr size_t index() const { return static_cast<size_t>(index_); }
  [[nodiscard]] constexpr int signed_index() const { return index_; }

 private:
  int index_;
};

template <typename InputT, int kMaxInputHistory, int kMaxPlayerCount>
class InputManager {
 public:
  InputManager() {
    inputs_.resize(kMaxInputHistory);
  }
  [[nodiscard]] InputT input(PlayerNumber player_number,
                             Frame current_frame) const {
    return inputs_[current_frame.index()][player_number.index()];
  }

  void set_input(PlayerNumber player_number, InputT input, Frame current_frame) {
    inputs_[current_frame.index()][player_number.index()] = input;
    //TODO repeat input up to the end of the array (speculative inputs)
  }

  [[nodiscard]] std::pair<std::array<InputT, kMaxInputHistory>, uint8_t> inputs(
      PlayerNumber player_number, Frame current_frame) const {
    (void)player_number;
    (void)current_frame;
    throw std::runtime_error("inputs() not implemented");
  }

  [[nodiscard]] std::array<InputT, kMaxPlayerCount> inputs(
      Frame current_frame) const{ return inputs_[current_frame.index()]; }

  [[nodiscard]] Frame last_confirm_frame() const { return last_confirm_frame_; }
  [[nodiscard]] Frame last_received_frame() const {
    auto smaller_frame = Frame(-1);
    for (int player_index = 0; player_index < kMaxPlayerCount; ++player_index) {
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

  void set_inputs_from_network(PlayerNumber player_number,
                               std::span<InputT> inputs, Frame frame) {
    (void)player_number;
    (void)inputs;
    (void)frame;
    throw std::runtime_error("set_inputs_from_network() not implemented");
  }

  [[nodiscard]] bool is_valid(PlayerNumber player_number) const {
    return input_metadata_[player_number.index()].is_valid;
  }
  [[nodiscard]] bool is_dirty() const { return is_dirty_; }

 private:
  struct InputMetadata {
    Frame last_received_frame = Frame(-1);
    bool is_valid = false;
  };
  std::vector<std::array<InputT, kMaxPlayerCount>> inputs_;
  std::array<InputMetadata, kMaxPlayerCount> input_metadata_{};
  Frame last_confirm_frame_{-1};
  bool is_dirty_ = false;
};

}  // namespace common

#endif  // GPR924_ENGINE_INPUT_MANAGER_H
