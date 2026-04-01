
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
// Requires a game model to support rollback from a confirmed state, input
// injection, and tick-based simulation.
template <typename ModelT, typename PlayerInputT, int kMaxPlayerCount>
concept RollbackableModel =
    requires(const ModelT& confirm_model, ModelT& speculative_model, std::span<const PlayerInputT, kMaxPlayerCount> player_input) {
      { speculative_model.RollbackFrom(confirm_model) };
      { speculative_model.Tick() };
      { speculative_model.set_inputs(player_input) };
    };
// Requires a game model to produce checksums for desync detection.
template <typename GameModelT, int kChecksumSystemCount>
concept ConfirmableGameModel = requires(const GameModelT& game_model) {
  {
    game_model.checksums()
  } -> std::convertible_to<Checksum<kChecksumSystemCount>>;
};

// Orchestrates rollback netcode by managing a confirmed game model, an input
// manager, and providing rollback-and-resimulate logic.
template <typename GameModelT, typename PlayerInputT, int kMaxInputHistory,
          int kMaxPlayerCount, int kChecksumSystemCount>
class RollbackManager {
 public:
  // Advances the confirmed game model by one frame using confirmed inputs and
  // returns the resulting checksums for desync detection.
  Checksum<kChecksumSystemCount> ConfirmLastFrame() {
    confirm_game_model_.set_inputs(input_manager_.inputs(
        Frame{input_manager_.last_confirm_frame().signed_index() + 1}));
    confirm_game_model_.Tick();
    input_manager_.ConfirmFrame();
    return confirm_game_model_.checksums();
  }

  // Resets the given game model to the confirmed state and re-simulates all
  // frames from the last confirmed frame up to the current frame using stored
  // inputs.
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
  static_assert(
      RollbackableModel<GameModelT, PlayerInputT, kMaxPlayerCount>,
      "GameModel needs to implement RollbackFrom(), set_inputs(), and Tick()");
  static_assert(ConfirmableGameModel<GameModelT, kChecksumSystemCount>,
                "GameModel should have a checksums() method that calculates "
                "the current checksums");
  GameModelT confirm_game_model_;
  InputManager<PlayerInputT, kMaxInputHistory, kMaxPlayerCount> input_manager_;
};

}  // namespace common

#endif  // GPR924_ENGINE_ROLLBACK_MANAGER_H
