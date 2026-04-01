
#ifndef GPR924_ENGINE_ROLLBACK_MANAGER_H
#define GPR924_ENGINE_ROLLBACK_MANAGER_H
#include "checksum.h"
#include "input_manager.h"

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

namespace common {
template <typename ModelT>
concept RollbackableModel =
    requires(const ModelT& confirm_model, ModelT& speculative_model) {
      { speculative_model.RollbackFrom(confirm_model) };
    };
template <typename GameModelT, int kChecksumSystemCount>
concept ConfirmableGameModel = requires(const GameModelT& game_model) {
  {
    game_model.checksums()
  } -> std::convertible_to<Checksum<kChecksumSystemCount>>;
};

template <RollbackableModel GameModelT, typename PlayerInputT,
          int kMaxInputHistory, int kMaxPlayerCount, int kChecksumSystemCount>
class RollbackManager {
 public:
  Checksum<kChecksumSystemCount> ConfirmLastFrame() {
    confirm_game_model_.set_inputs(input_manager_.inputs(
        Frame{input_manager_.last_confirm_frame().signed_index() + 1}));
    confirm_game_model_.Tick();
    input_manager_.ConfirmFrame();
    return confirm_game_model_.checksums();
  }

  void RollbackAndResimulate(GameModelT& current_game_model,
                              Frame current_frame) {
    current_game_model.RollbackFrom(confirm_game_model_);
    const auto delta =
        current_frame.signed_index() - input_manager_.last_confirm_frame().signed_index();
    for (int i = 0; i < delta; ++i) {
      current_game_model.set_inputs(input_manager_.inputs(
          Frame{input_manager_.last_confirm_frame().signed_index() + i + 1}));
      current_game_model.Tick();
    }
  }
  auto& input_manager() { return input_manager_; }
  const auto& input_manager() const { return input_manager_; }

 private:
  static_assert(ConfirmableGameModel<GameModelT, kChecksumSystemCount>,
                "GameModel should have a checksums() method that calculates "
                "the current checksums");
  GameModelT confirm_game_model_;
  InputManager<PlayerInputT, kMaxInputHistory, kMaxPlayerCount> input_manager_;
};

}  // namespace common

#endif  // GPR924_ENGINE_ROLLBACK_MANAGER_H
