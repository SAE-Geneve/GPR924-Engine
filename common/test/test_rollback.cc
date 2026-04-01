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

#include "rollback/rollback_manager.h"

#include <array>
#include <span>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using PlayerInput = int;
static constexpr auto kMaxInputHistory = 10;
static constexpr auto kMaxPlayerCount = 2;
static constexpr auto kChecksumSystemCount = 1;

// ---------------------------------------------------------------------------
// Simple GameModel satisfying RollbackableModel + ConfirmableGameModel
// ---------------------------------------------------------------------------

struct SimpleGameModel {
  int index = 0;
  std::array<PlayerInput, kMaxPlayerCount> current_inputs{};

  void Tick() {
    for (auto input : current_inputs) {
      index += input;
    }
  }

  void set_inputs(std::span<const PlayerInput, kMaxPlayerCount> inputs) {
    std::ranges::copy(inputs, current_inputs.begin());
  }

  void RollbackFrom(const SimpleGameModel& other) { index = other.index; }

  [[nodiscard]] common::Checksum<kChecksumSystemCount> checksums() const {
    return {static_cast<uint32_t>(index)};
  }
};

using TestRollbackManager =
    common::RollbackManager<SimpleGameModel, PlayerInput, kMaxInputHistory,
                            kMaxPlayerCount, kChecksumSystemCount>;

// Helper: tick the speculative model with inputs from the manager at a frame.
static void SpeculativeTick(SimpleGameModel& model,
                            TestRollbackManager& manager,
                            common::Frame frame) {
  model.set_inputs(std::span<const PlayerInput, kMaxPlayerCount>(manager.input_manager().inputs(frame)));
  model.Tick();
}

// ---------------------------------------------------------------------------
// Core rollback flow
// ---------------------------------------------------------------------------

TEST(RollbackManager, ConfirmLastFrame_AdvancesConfirmModel) {
  TestRollbackManager manager;
  manager.input_manager().set_input(common::PlayerNumber{0}, 3,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 7,
                                    common::Frame{0});

  auto checksum = manager.ConfirmLastFrame();
  // Confirm model ticked with {3, 7} -> index = 10
  EXPECT_EQ(checksum[0], 10u);
}

TEST(RollbackManager, ConfirmLastFrame_ReturnsCorrectChecksum) {
  TestRollbackManager manager;
  manager.input_manager().set_input(common::PlayerNumber{0}, 5,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 0,
                                    common::Frame{0});

  auto checksum = manager.ConfirmLastFrame();
  EXPECT_EQ(checksum[0], 5u);
}

TEST(RollbackManager, RollbackAndResimulate_RestoresFromConfirm) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: inputs {5, 5} -> index = 10
  manager.input_manager().set_input(common::PlayerNumber{0}, 5,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 5,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});
  ASSERT_EQ(speculative.index, 10);

  // Frame 1: speculative (wrong) inputs {100, 100} -> index = 210
  manager.input_manager().set_input(common::PlayerNumber{0}, 100,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 100,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});
  ASSERT_EQ(speculative.index, 210);

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Correct inputs arrive for frame 1
  manager.input_manager().set_input(common::PlayerNumber{0}, 1,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 1,
                                    common::Frame{1});

  // Rollback and resimulate (last ticked frame = 1)
  manager.RollbackAndResimulate(speculative, common::Frame{1});
  // After rollback: index = 10 (confirm state after frame 0)
  // Resimulate frame 1 with {1, 1}: index = 10 + 2 = 12
  EXPECT_EQ(speculative.index, 12);
}

// ---------------------------------------------------------------------------
// Multiple confirms
// ---------------------------------------------------------------------------

TEST(RollbackManager, MultipleConfirms_ProgressState) {
  TestRollbackManager manager;

  // Frame 0: {2, 3}
  manager.input_manager().set_input(common::PlayerNumber{0}, 2,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 3,
                                    common::Frame{0});
  // Frame 1: {4, 1}
  manager.input_manager().set_input(common::PlayerNumber{0}, 4,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 1,
                                    common::Frame{1});

  auto checksum0 = manager.ConfirmLastFrame();
  // After frame 0: index = 2 + 3 = 5
  EXPECT_EQ(checksum0[0], 5u);

  auto checksum1 = manager.ConfirmLastFrame();
  // After frame 1: index = 5 + 4 + 1 = 10
  EXPECT_EQ(checksum1[0], 10u);
}

TEST(RollbackManager, RollbackAfterMultipleConfirms) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {1, 1} -> index = 2
  manager.input_manager().set_input(common::PlayerNumber{0}, 1,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 1,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Frame 1: {2, 2} -> index = 6
  manager.input_manager().set_input(common::PlayerNumber{0}, 2,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 2,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});

  // Frame 2: speculative (wrong) {10, 10} -> index = 26
  manager.input_manager().set_input(common::PlayerNumber{0}, 10,
                                    common::Frame{2});
  manager.input_manager().set_input(common::PlayerNumber{1}, 10,
                                    common::Frame{2});
  SpeculativeTick(speculative, manager, common::Frame{2});
  ASSERT_EQ(speculative.index, 26);

  // Confirm frames 0 and 1
  manager.ConfirmLastFrame();
  manager.ConfirmLastFrame();

  // Correct inputs arrive for frame 2
  manager.input_manager().set_input(common::PlayerNumber{0}, 3,
                                    common::Frame{2});
  manager.input_manager().set_input(common::PlayerNumber{1}, 3,
                                    common::Frame{2});

  manager.RollbackAndResimulate(speculative, common::Frame{2});
  // Confirm state: index = 6 (after frames 0 and 1)
  // Resimulate frame 2 with {3, 3}: index = 6 + 6 = 12
  EXPECT_EQ(speculative.index, 12);
}

// ---------------------------------------------------------------------------
// Rollback after speculative frames
// ---------------------------------------------------------------------------

TEST(RollbackManager, RollbackAfterSeveralSpeculativeFrames) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {1, 0}
  manager.input_manager().set_input(common::PlayerNumber{0}, 1,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 0,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Frames 1-3: speculative with {1, 0} (carried forward)
  for (int f = 1; f <= 3; ++f) {
    SpeculativeTick(speculative, manager, common::Frame{f});
  }
  // index = 1 + 1 + 1 + 1 = 4
  ASSERT_EQ(speculative.index, 4);

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Correct inputs for frames 1-3: {2, 0}
  for (int f = 1; f <= 3; ++f) {
    manager.input_manager().set_input(common::PlayerNumber{0}, 2,
                                      common::Frame{f});
  }

  manager.RollbackAndResimulate(speculative, common::Frame{3});
  // Confirm state: index = 1 (after frame 0)
  // Resimulate frames 1-3 with {2, 0}: 1 + 2 + 2 + 2 = 7
  EXPECT_EQ(speculative.index, 7);
}

TEST(RollbackManager, RollbackResetsToConfirmState) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {10, 10} -> index = 20
  manager.input_manager().set_input(common::PlayerNumber{0}, 10,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 10,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Speculative goes wild: {99, 99}
  manager.input_manager().set_input(common::PlayerNumber{0}, 99,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 99,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});
  ASSERT_EQ(speculative.index, 218);

  // After rollback with no extra frames to resim, state = confirm state
  manager.input_manager().set_input(common::PlayerNumber{0}, 0,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 0,
                                    common::Frame{1});
  manager.RollbackAndResimulate(speculative, common::Frame{1});
  // Confirm state = 20, resim frame 1 with {0, 0}: 20 + 0 = 20
  EXPECT_EQ(speculative.index, 20);
}

// ---------------------------------------------------------------------------
// Checksum verification
// ---------------------------------------------------------------------------

TEST(RollbackManager, ChecksumMatchesAfterIdenticalInputs) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {4, 6}
  manager.input_manager().set_input(common::PlayerNumber{0}, 4,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 6,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  auto confirm_checksum = manager.ConfirmLastFrame();
  auto speculative_checksum = speculative.checksums();

  // Both should be at index = 10
  EXPECT_EQ(static_cast<uint32_t>(confirm_checksum),
            static_cast<uint32_t>(speculative_checksum));
}

TEST(RollbackManager, ChecksumDiffersAfterDifferentInputs) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {4, 6} for confirm
  manager.input_manager().set_input(common::PlayerNumber{0}, 4,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 6,
                                    common::Frame{0});

  auto confirm_checksum = manager.ConfirmLastFrame();

  // Speculative model uses different inputs: {1, 1}
  speculative.set_inputs(std::span<const PlayerInput, kMaxPlayerCount>({1, 1}));
  speculative.Tick();
  auto speculative_checksum = speculative.checksums();

  // Confirm index = 10, Speculative index = 2 -> different checksums
  EXPECT_NE(static_cast<uint32_t>(confirm_checksum),
            static_cast<uint32_t>(speculative_checksum));
}

// ---------------------------------------------------------------------------
// Multi-player scenarios
// ---------------------------------------------------------------------------

TEST(RollbackManager, TwoPlayerInputsAccumulate) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: player 0 inputs 3, player 1 inputs 7
  manager.input_manager().set_input(common::PlayerNumber{0}, 3,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 7,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  // index = 3 + 7 = 10
  EXPECT_EQ(speculative.index, 10);
}

TEST(RollbackManager, RollbackWithTwoPlayers) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {5, 5} -> index = 10
  manager.input_manager().set_input(common::PlayerNumber{0}, 5,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 5,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Frame 1: speculative player 0 = 10, player 1 = 10 (wrong)
  manager.input_manager().set_input(common::PlayerNumber{0}, 10,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 10,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});
  ASSERT_EQ(speculative.index, 30);

  // Correct: player 0 = 1, player 1 stays 10
  manager.input_manager().set_input(common::PlayerNumber{0}, 1,
                                    common::Frame{1});

  manager.RollbackAndResimulate(speculative, common::Frame{1});
  // Confirm state = 10, resim frame 1 with {1, 10}: 10 + 11 = 21
  EXPECT_EQ(speculative.index, 21);
}

// ---------------------------------------------------------------------------
// Dirty flag integration
// ---------------------------------------------------------------------------

TEST(RollbackManager, DirtyFlagAfterNetworkInputChange) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: speculative {5, 5} -> index = 10
  manager.input_manager().set_input(common::PlayerNumber{0}, 5,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 5,
                                    common::Frame{0});
  manager.input_manager().set_valid(common::PlayerNumber{0}, true);
  manager.input_manager().set_valid(common::PlayerNumber{1}, true);
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Frame 1: speculative {3, 3}
  manager.input_manager().set_input(common::PlayerNumber{0}, 3,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 3,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});
  ASSERT_EQ(speculative.index, 16);

  // Network says frame 1 should be {7, 7} (different from speculative)
  std::vector<PlayerInput> net0 = {7};
  std::vector<PlayerInput> net1 = {7};
  manager.input_manager().set_inputs_from_network(
      common::PlayerNumber{0}, std::span<PlayerInput>(net0), common::Frame{1});
  manager.input_manager().set_inputs_from_network(
      common::PlayerNumber{1}, std::span<PlayerInput>(net1), common::Frame{1});

  ASSERT_TRUE(manager.input_manager().is_dirty());

  // Rollback and resimulate with corrected inputs
  manager.RollbackAndResimulate(speculative, common::Frame{1});
  // Confirm state = 10, resim frame 1 with {7, 7}: 10 + 14 = 24
  EXPECT_EQ(speculative.index, 24);

  manager.input_manager().CleanDirty();
  EXPECT_FALSE(manager.input_manager().is_dirty());
}

TEST(RollbackManager, NoDirtyNoRollbackNeeded) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // Frame 0: {5, 5}
  manager.input_manager().set_input(common::PlayerNumber{0}, 5,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 5,
                                    common::Frame{0});
  manager.input_manager().set_valid(common::PlayerNumber{0}, true);
  manager.input_manager().set_valid(common::PlayerNumber{1}, true);
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Frame 1: speculative {3, 3}
  manager.input_manager().set_input(common::PlayerNumber{0}, 3,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 3,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});

  // Network confirms the same inputs
  std::vector<PlayerInput> net0 = {3};
  std::vector<PlayerInput> net1 = {3};
  manager.input_manager().set_inputs_from_network(
      common::PlayerNumber{0}, std::span<PlayerInput>(net0), common::Frame{1});
  manager.input_manager().set_inputs_from_network(
      common::PlayerNumber{1}, std::span<PlayerInput>(net1), common::Frame{1});

  // Not dirty -> no rollback needed
  EXPECT_FALSE(manager.input_manager().is_dirty());
  EXPECT_EQ(speculative.index, 16);
}

// ---------------------------------------------------------------------------
// Confirm-then-rollback cycles
// ---------------------------------------------------------------------------

TEST(RollbackManager, ConfirmThenRollbackCycle) {
  TestRollbackManager manager;
  SimpleGameModel speculative;

  // --- Cycle 1 ---
  // Frame 0: {1, 1} -> index = 2
  manager.input_manager().set_input(common::PlayerNumber{0}, 1,
                                    common::Frame{0});
  manager.input_manager().set_input(common::PlayerNumber{1}, 1,
                                    common::Frame{0});
  SpeculativeTick(speculative, manager, common::Frame{0});

  // Frame 1: speculative {5, 5} -> index = 12
  manager.input_manager().set_input(common::PlayerNumber{0}, 5,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 5,
                                    common::Frame{1});
  SpeculativeTick(speculative, manager, common::Frame{1});
  ASSERT_EQ(speculative.index, 12);

  // Confirm frame 0
  manager.ConfirmLastFrame();

  // Correct inputs for frame 1: {2, 2}
  manager.input_manager().set_input(common::PlayerNumber{0}, 2,
                                    common::Frame{1});
  manager.input_manager().set_input(common::PlayerNumber{1}, 2,
                                    common::Frame{1});

  manager.RollbackAndResimulate(speculative, common::Frame{1});
  // Confirm = 2, resim frame 1 with {2, 2}: 2 + 4 = 6
  EXPECT_EQ(speculative.index, 6);

  // --- Cycle 2 ---
  // Frame 2: speculative {10, 10} -> index = 26
  manager.input_manager().set_input(common::PlayerNumber{0}, 10,
                                    common::Frame{2});
  manager.input_manager().set_input(common::PlayerNumber{1}, 10,
                                    common::Frame{2});
  SpeculativeTick(speculative, manager, common::Frame{2});
  ASSERT_EQ(speculative.index, 26);

  // Confirm frame 1
  manager.ConfirmLastFrame();

  // Correct inputs for frame 2: {3, 3}
  manager.input_manager().set_input(common::PlayerNumber{0}, 3,
                                    common::Frame{2});
  manager.input_manager().set_input(common::PlayerNumber{1}, 3,
                                    common::Frame{2});

  manager.RollbackAndResimulate(speculative, common::Frame{2});
  // Confirm = 6 (after frames 0 and 1), resim frame 2 with {3, 3}: 6 + 6 = 12
  EXPECT_EQ(speculative.index, 12);
}
