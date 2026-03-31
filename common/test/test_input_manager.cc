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

#include "rollback/input_manager.h"

#include "gmock/gmock.h"
#include "gtest/gtest.h"

using PlayerInput = int;
static constexpr auto kMaxInputHistory = 5;
static constexpr auto kMaxPlayerCount = 2;

using TestInputManager = common::InputManager<PlayerInput, kMaxInputHistory, kMaxPlayerCount>;

TEST(InputManager, SetSingleInput) {
  TestInputManager manager;
  static constexpr auto kPlayerNumber = common::PlayerNumber{1};
  static constexpr auto kFrame = common::Frame{1};
  static constexpr auto kInput = -52;
  manager.set_input(kPlayerNumber, kInput, kFrame);

  EXPECT_EQ(manager.input(kPlayerNumber, kFrame), kInput);
}