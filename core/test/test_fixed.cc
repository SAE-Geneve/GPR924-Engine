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
#include "gtest/gtest.h"
#include "maths/fixed.h"

TEST(Fixed, Construct) {
  constexpr std::int32_t i = 32;
  constexpr auto f = core::Fixed{i};
  EXPECT_EQ(i, static_cast<std::int32_t>(f));
}

struct FixedIntOperationFixture
    : public ::testing::TestWithParam<std::pair<int, int>> {};

TEST_P(FixedIntOperationFixture, Add) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 + f2;
  EXPECT_EQ(i1 + i2, static_cast<std::int32_t>(result));
}

TEST_P(FixedIntOperationFixture, Sub) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 - f2;
  EXPECT_EQ(i1 - i2, static_cast<std::int32_t>(result));

  const auto neg1 = -f1;
  const auto neg2 = -f2;
  EXPECT_EQ(-i1, static_cast<std::int32_t>(neg1));
  EXPECT_EQ(-i2, static_cast<std::int32_t>(neg2));
}

TEST_P(FixedIntOperationFixture, Mul) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 * f2;
  EXPECT_EQ(i1 * i2, static_cast<std::int32_t>(result));
}

static constexpr float kEpsilon = 0.0001f;
TEST_P(FixedIntOperationFixture, Div) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 / f2;
  EXPECT_NEAR(static_cast<float>(i1) / static_cast<float>(i2),
              static_cast<float>(result), kEpsilon);
}

INSTANTIATE_TEST_SUITE_P(PositiveNumbers, FixedIntOperationFixture,
                         testing::Values(std::pair{15, 3}, std::pair{1, 2},
                                         std::pair{23, 32}, std::pair{0, 32}));

INSTANTIATE_TEST_SUITE_P(NegativeNumbers, FixedIntOperationFixture,
                         testing::Values(std::pair{-1, 1}, std::pair{-1, -1},
                                         std::pair{-25, 7},
                                         std::pair{23, -32}));

struct FixedFloatOperationFixture
    : public ::testing::TestWithParam<std::pair<float, float>> {};

TEST_P(FixedFloatOperationFixture, Add) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 + f2;
  EXPECT_NEAR(i1 + i2, static_cast<float>(result), kEpsilon);
}

TEST_P(FixedFloatOperationFixture, Sub) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 - f2;
  EXPECT_NEAR(i1 - i2, static_cast<float>(result), kEpsilon);
}

TEST_P(FixedFloatOperationFixture, Mul) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 * f2;
  EXPECT_NEAR(i1 * i2, static_cast<float>(result), kEpsilon);
}

TEST_P(FixedFloatOperationFixture, Div) {
  auto [i1, i2] = GetParam();
  const auto f1 = core::Fixed{i1};
  const auto f2 = core::Fixed{i2};
  const auto result = f1 / f2;
  EXPECT_NEAR(i1 / i2, static_cast<float>(result), kEpsilon);
}

INSTANTIATE_TEST_SUITE_P(PositiveNumbers, FixedFloatOperationFixture,
                         testing::Values(std::pair{1.0f, 2.0f},
                                         std::pair{10.0f, 5.0f},
                                         std::pair{1.5f, 3.2f},
                                         std::pair{0.0f, 4.3f}));

INSTANTIATE_TEST_SUITE_P(NegativeNumbers, FixedFloatOperationFixture,
                         testing::Values(std::pair{-1.0f, 2.0f},
                                         std::pair{-1.0f, -1.0f},
                                         std::pair{-25.0f, 7.0f},
                                         std::pair{23.0f, -32.0f}));

