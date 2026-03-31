
#ifndef GPR924_ENGINE_ROLLBACK_MANAGER_H
#define GPR924_ENGINE_ROLLBACK_MANAGER_H
#include "checksum.h"

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
template <typename GameModelT>
concept game_model = requires(const GameModelT& confirm_game_model,
                              GameModelT& speculative_game_model) {
  { speculative_game_model.RollbackFrom(confirm_game_model) };
};
template <typename GameModelT, int kChecksumSystemCount>
concept confirmable_game_model = requires(const GameModelT& game_model) {
  {
    game_model.checksums()
  } -> std::convertible_to<Checksum<kChecksumSystemCount>>;
};

template <game_model GameModelT, int kChecksumSystemCount>
class RollbackManager {
 public:
  Checksum<kChecksumSystemCount> ConfirmLastFrame() {
    confirm_game_model_.Tick();
    return confirm_game_model_.checksums();
  }

  void RollbackToConfirmFrame(GameModelT& current_game_model) {
    current_game_model.RollbackFrom(confirm_game_model_);
  }

 private:
  static_assert(confirmable_game_model<GameModelT, kChecksumSystemCount>,
                "GameModel should have a checksums() method that calculates "
                "the current checksums");
  GameModelT confirm_game_model_;
};

}  // namespace common

#endif  // GPR924_ENGINE_ROLLBACK_MANAGER_H
