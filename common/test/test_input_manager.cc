/*
Copyright 2026 SAE Institute Switzerland AG

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

#include "rollback/input_manager.h"

#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using PlayerInput = int;
static constexpr auto kMaxInputHistory = 5;
static constexpr auto kMaxPlayerCount = 2;

using TestInputManager =
    common::InputManager<PlayerInput, kMaxInputHistory, kMaxPlayerCount>;

// ---------------------------------------------------------------------------
// Basic set / get
// ---------------------------------------------------------------------------

TEST(InputManager, SetSingleInput) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{1};
  static constexpr auto kFrame = common::Frame{1};
  static constexpr auto kInput = -52;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  EXPECT_EQ(manager.input(kPlayerNumber, kFrame), kInput);
}

TEST(InputManager, BoundaryCondition_Frame0) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  static constexpr auto kFrame = common::Frame{0};
  static constexpr auto kInput = 42;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  EXPECT_EQ(manager.input(kPlayerNumber, kFrame), kInput);
}

TEST(InputManager, BoundaryCondition_MaxFrame) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  static constexpr auto kFrame = common::Frame{kMaxInputHistory - 1};
  static constexpr auto kInput = 99;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  EXPECT_EQ(manager.input(kPlayerNumber, kFrame), kInput);
}

TEST(InputManager, BoundaryCondition_PlayerIndex0) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  static constexpr auto kFrame = common::Frame{2};
  static constexpr auto kInput = 7;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  EXPECT_EQ(manager.input(kPlayerNumber, kFrame), kInput);
}


TEST(InputManager, GetMoreRecentInput) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  static constexpr auto kFrame = common::Frame{2};
  static constexpr auto kNextFrame = common::Frame{3};
  static constexpr auto kInput = 7;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  EXPECT_EQ(manager.input(kPlayerNumber, kNextFrame), kInput);
}

// ---------------------------------------------------------------------------
// inputs(Frame) — all players at a given frame
// ---------------------------------------------------------------------------

TEST(InputManager, GetAllPlayersInputsForFrame) {
  TestInputManager manager;
  static constexpr auto kFrame = common::Frame{2};
  static constexpr auto kInput0 = 10;
  static constexpr auto kInput1 = 20;
  manager.set_input(common::PlayerNumber{0}, kInput0, kFrame);
  manager.set_input(common::PlayerNumber{1}, kInput1, kFrame);

  const auto inputs = manager.inputs(kFrame);
  EXPECT_EQ(inputs[0], kInput0);
  EXPECT_EQ(inputs[1], kInput1);
}

// ---------------------------------------------------------------------------
// last_received_frame per player — updated by set_input
// ---------------------------------------------------------------------------

TEST(InputManager, LastReceivedFramePerPlayer_DefaultIsNegativeOne) {
  TestInputManager manager;
  EXPECT_EQ(manager.last_received_frame(common::PlayerNumber{0}).signed_index(),
            -1);
  EXPECT_EQ(manager.last_received_frame(common::PlayerNumber{1}).signed_index(),
            -1);
}

TEST(InputManager, LastReceivedFramePerPlayer_UpdatedOnSetInput) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 42, common::Frame{2});

  EXPECT_EQ(manager.last_received_frame(kPlayerNumber).signed_index(), 2);
}

TEST(InputManager, LastReceivedFramePerPlayer_OnlyUpdatesForNewerFrame) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  // Set a later frame first, then an earlier one.
  manager.set_input(kPlayerNumber, 42, common::Frame{3});
  manager.set_input(kPlayerNumber, 10, common::Frame{1});

  // last_received_frame must not regress.
  EXPECT_EQ(manager.last_received_frame(kPlayerNumber).signed_index(), 3);
}

// ---------------------------------------------------------------------------
// last_received_frame global — minimum across all valid players
// ---------------------------------------------------------------------------

TEST(InputManager, LastReceivedFrame_Global_NoValidPlayers) {
  TestInputManager manager;
  EXPECT_EQ(manager.last_received_frame().signed_index(), -1);
}

TEST(InputManager, LastReceivedFrame_Global_ReturnsMinimumAcrossValidPlayers) {
  TestInputManager manager;
  manager.set_valid(common::PlayerNumber{0}, true);
  manager.set_valid(common::PlayerNumber{1}, true);
  manager.set_input(common::PlayerNumber{0}, 10, common::Frame{3});
  manager.set_input(common::PlayerNumber{1}, 20, common::Frame{1});

  // Both valid — minimum last_received_frame is 1.
  EXPECT_EQ(manager.last_received_frame().signed_index(), 1);
}

TEST(InputManager, LastReceivedFrame_Global_IgnoresInvalidPlayers) {
  TestInputManager manager;
  // Only player 0 is valid.
  manager.set_valid(common::PlayerNumber{0}, true);
  manager.set_input(common::PlayerNumber{0}, 10, common::Frame{3});

  // Player 1 is not valid — should not drag the minimum to -1.
  EXPECT_EQ(manager.last_received_frame().signed_index(), 3);
}

// ---------------------------------------------------------------------------
// is_valid
// ---------------------------------------------------------------------------

TEST(InputManager, IsValid_DefaultFalse) {
  TestInputManager manager;
  EXPECT_FALSE(manager.is_valid(common::PlayerNumber{0}));
  EXPECT_FALSE(manager.is_valid(common::PlayerNumber{1}));
}

TEST(InputManager, IsValid_SetterWorks) {
  TestInputManager manager;
  manager.set_valid(common::PlayerNumber{0}, true);

  EXPECT_TRUE(manager.is_valid(common::PlayerNumber{0}));
  EXPECT_FALSE(manager.is_valid(common::PlayerNumber{1}));
}

TEST(InputManager, IsValid_CanBeCleared) {
  TestInputManager manager;
  manager.set_valid(common::PlayerNumber{0}, true);
  manager.set_valid(common::PlayerNumber{0}, false);

  EXPECT_FALSE(manager.is_valid(common::PlayerNumber{0}));
}

// ---------------------------------------------------------------------------
// is_dirty — set when received input differs from stored value
// ---------------------------------------------------------------------------

TEST(InputManager, IsDirty_DefaultFalse) {
  TestInputManager manager;
  EXPECT_FALSE(manager.is_dirty());
}

TEST(InputManager, IsDirty_NotSetOnFirstInput) {
  TestInputManager manager;
  // Writing a value where nothing was set before (default 0) with value 0.
  manager.set_input(common::PlayerNumber{0}, 0, common::Frame{1});
  EXPECT_FALSE(manager.is_dirty());
}


// ---------------------------------------------------------------------------
// CleanDirty — resets the dirty flag
// ---------------------------------------------------------------------------

TEST(InputManager, CleanDirty_ResetsToFalse) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 99, common::Frame{0});

  std::vector<PlayerInput> network_inputs = {10};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{0});
  ASSERT_TRUE(manager.is_dirty());

  manager.CleanDirty();
  EXPECT_FALSE(manager.is_dirty());
}

TEST(InputManager, CleanDirty_NoOpWhenAlreadyClean) {
  TestInputManager manager;
  ASSERT_FALSE(manager.is_dirty());

  manager.CleanDirty();
  EXPECT_FALSE(manager.is_dirty());
}

TEST(InputManager, CleanDirty_DirtyCanBeSetAgainAfterCleaning) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 99, common::Frame{0});

  std::vector<PlayerInput> network_inputs = {10};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{0});
  manager.CleanDirty();
  ASSERT_FALSE(manager.is_dirty());

  // Trigger dirty again with a different mismatch at frame 1.
  manager.set_input(kPlayerNumber, 50, common::Frame{1});
  std::vector<PlayerInput> network_inputs2 = {77};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs2),
      common::Frame{1});
  EXPECT_TRUE(manager.is_dirty());
}

// ---------------------------------------------------------------------------
// Speculative inputs — input is copied forward to subsequent frames
// ---------------------------------------------------------------------------

TEST(InputManager, SpeculativeInput_CopiedToSubsequentFrames) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  static constexpr auto kFrame = common::Frame{1};
  static constexpr auto kInput = 42;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  for (int f = kFrame.signed_index() + 1; f < kMaxInputHistory; ++f) {
    EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{f}), kInput)
        << "speculative input missing at frame " << f;
  }
}

TEST(InputManager, SpeculativeInput_NotOverwrittenByOlderFrame) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  static constexpr auto kLatestInput = 42;
  static constexpr auto kOlderInput = 10;

  manager.set_input(kPlayerNumber, kLatestInput, common::Frame{3});
  // Setting an older frame must not overwrite the already-speculated later frames.
  manager.set_input(kPlayerNumber, kOlderInput, common::Frame{1});

  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{3}), kLatestInput);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{4}), kLatestInput);
}

// ---------------------------------------------------------------------------
// inputs(PlayerNumber, Frame) — history for one player (TDD: not yet implemented)
// ---------------------------------------------------------------------------

TEST(InputManager, InputsHistory_ReturnsChronologicalHistory) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});
  manager.set_input(kPlayerNumber, 20, common::Frame{1});
  manager.set_input(kPlayerNumber, 30, common::Frame{2});

  const auto history=
      manager.inputs(kPlayerNumber, common::Frame{2});

  EXPECT_EQ(history.size(), 3u);
  EXPECT_EQ(history[0], 10);
  EXPECT_EQ(history[1], 20);
  EXPECT_EQ(history[2], 30);
}

TEST(InputManager, InputsHistory_CountLimitedByMaxHistory) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  for (int f = 0; f < kMaxInputHistory; ++f) {
    manager.set_input(kPlayerNumber, f * 10, common::Frame{f});
  }

  const auto history =
      manager.inputs(kPlayerNumber, common::Frame{kMaxInputHistory - 1});

  EXPECT_EQ(history.size(), static_cast<uint8_t>(kMaxInputHistory));
}

// ---------------------------------------------------------------------------
// set_inputs_from_network
// Semantics: `current_frame` is the LAST frame in the span; inputs cover
// [current_frame - span.size() + 1, current_frame].
// ---------------------------------------------------------------------------

TEST(InputManager, SetInputsFromNetwork_SetsMultipleFrames) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  std::vector<PlayerInput> network_inputs = {10, 20, 30};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{2});

  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{0}), 10);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{1}), 20);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{2}), 30);
}

TEST(InputManager, SetInputsFromNetwork_UpdatesLastReceivedFrame) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  std::vector<PlayerInput> network_inputs = {10, 20, 30};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{2});

  // current_frame is the last frame in the span, so last_received = 2
  EXPECT_EQ(manager.last_received_frame(kPlayerNumber).signed_index(), 2);
}

TEST(InputManager, SetInputsFromNetwork_SetsDirtyWhenInputsDiffer) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  // Establish a speculative input first.
  manager.set_input(kPlayerNumber, 99, common::Frame{0});

  // Receive a different input from the network for the same frame.
  std::vector<PlayerInput> network_inputs = {10};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{0});

  EXPECT_TRUE(manager.is_dirty());
}

TEST(InputManager, SetInputsFromNetwork_NotDirtyWhenInputsMatch) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});

  std::vector<PlayerInput> network_inputs = {10};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{0});

  EXPECT_FALSE(manager.is_dirty());
}

// ---------------------------------------------------------------------------
// ConfirmFrame — advances the sliding window
// ---------------------------------------------------------------------------

TEST(InputManager, ConfirmFrame_IncrementsLastConfirmFrame) {
  TestInputManager manager;
  EXPECT_EQ(manager.last_confirm_frame().signed_index(), -1);

  manager.ConfirmFrame();
  EXPECT_EQ(manager.last_confirm_frame().signed_index(), 0);

  manager.ConfirmFrame();
  EXPECT_EQ(manager.last_confirm_frame().signed_index(), 1);
}

TEST(InputManager, ConfirmFrame_SlidesWindow) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});
  manager.set_input(kPlayerNumber, 20, common::Frame{1});
  manager.set_input(kPlayerNumber, 30, common::Frame{2});

  manager.ConfirmFrame();

  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{1}), 20);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{2}), 30);
}

TEST(InputManager, ConfirmFrame_InputAccessAfterMultipleConfirms) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  for (int f = 0; f < kMaxInputHistory; ++f) {
    manager.set_input(kPlayerNumber, (f + 1) * 10, common::Frame{f});
  }

  manager.ConfirmFrame();
  manager.ConfirmFrame();
  EXPECT_EQ(manager.last_confirm_frame().signed_index(), 1);

  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{2}), 30);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{3}), 40);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{4}), 50);
}

TEST(InputManager, ConfirmFrame_SetInputAfterConfirm) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});

  manager.ConfirmFrame();

  manager.set_input(kPlayerNumber, 77, common::Frame{1});
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{1}), 77);
}

TEST(InputManager, ConfirmFrame_InputsHistoryAfterConfirm) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});
  manager.set_input(kPlayerNumber, 20, common::Frame{1});
  manager.set_input(kPlayerNumber, 30, common::Frame{2});

  manager.ConfirmFrame();

  const auto history = manager.inputs(kPlayerNumber, common::Frame{2});
  EXPECT_EQ(history.size(), 2u);
  EXPECT_EQ(history[0], 20);
  EXPECT_EQ(history[1], 30);
}

// ---------------------------------------------------------------------------
// Dirty + Network integration
// ---------------------------------------------------------------------------

TEST(InputManager, DirtyNetworkFlow_SetFromNetworkThenClean) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 99, common::Frame{0});

  std::array network_inputs = {10};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{0});
  ASSERT_TRUE(manager.is_dirty());

  manager.CleanDirty();
  ASSERT_FALSE(manager.is_dirty());

  // Receiving the same input again should not re-trigger dirty.
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{0});
  EXPECT_FALSE(manager.is_dirty());
}

TEST(InputManager, DirtyNetworkFlow_MultiplePlayers) {
  TestInputManager manager;
  static constexpr auto kPlayer0 = common::PlayerNumber{0};
  static constexpr auto kPlayer1 = common::PlayerNumber{1};
  manager.set_input(kPlayer0, 99, common::Frame{0});
  manager.set_input(kPlayer1, 88, common::Frame{0});

  // Mismatch on player 0.
  std::array net0 = {10};
  manager.set_inputs_from_network(kPlayer0,
                                  std::span<PlayerInput>(net0),
                                  common::Frame{0});
  ASSERT_TRUE(manager.is_dirty());
  manager.CleanDirty();

  // Mismatch on player 1.
  std::array net1 = {20};
  manager.set_inputs_from_network(kPlayer1,
                                  std::span<PlayerInput>(net1),
                                  common::Frame{0});
  EXPECT_TRUE(manager.is_dirty());
}

TEST(InputManager, DirtyNetworkFlow_ConfirmThenNetworkInput) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});
  manager.set_input(kPlayerNumber, 50, common::Frame{1});

  manager.ConfirmFrame();

  // Receive a different input at frame 1 from the network.
  std::array network_inputs = {77};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{1});

  EXPECT_TRUE(manager.is_dirty());
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{1}), 77);
}

// ---------------------------------------------------------------------------
// Edge cases
// ---------------------------------------------------------------------------

TEST(InputManager, SetInput_ThrowsOnOverCapacity) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  EXPECT_THROW(
      manager.set_input(kPlayerNumber, 42, common::Frame{kMaxInputHistory}),
      std::runtime_error);
}

TEST(InputManager, ConfirmFrame_ThenSetInput_Interleaved) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});
  manager.set_input(kPlayerNumber, 20, common::Frame{1});
  manager.set_input(kPlayerNumber, 30, common::Frame{2});

  manager.ConfirmFrame();  // confirms frame 0
  manager.set_input(kPlayerNumber, 40, common::Frame{3});

  manager.ConfirmFrame();  // confirms frame 1
  manager.set_input(kPlayerNumber, 50, common::Frame{4});

  EXPECT_EQ(manager.last_confirm_frame().signed_index(), 1);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{2}), 30);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{3}), 40);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{4}), 50);
}

TEST(InputManager, SetInputsFromNetwork_MultipleFramesAfterConfirm) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});
  manager.set_input(kPlayerNumber, 20, common::Frame{1});
  manager.set_input(kPlayerNumber, 30, common::Frame{2});

  manager.ConfirmFrame();  // confirms frame 0

  std::array network_inputs = {55, 66};
  manager.set_inputs_from_network(
      kPlayerNumber, std::span<PlayerInput>(network_inputs), common::Frame{2});

  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{1}), 55);
  EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{2}), 66);
}

TEST(InputManager, SpeculativeInput_AfterConfirmFrame) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{0};
  manager.set_input(kPlayerNumber, 10, common::Frame{0});

  manager.ConfirmFrame();  // confirms frame 0

  manager.set_input(kPlayerNumber, 42, common::Frame{1});

  // Speculative input should fill forward from frame 1.
  for (int f = 2; f < kMaxInputHistory; ++f) {
    EXPECT_EQ(manager.input(kPlayerNumber, common::Frame{f}), 42)
        << "speculative input missing at frame " << f;
  }
}
