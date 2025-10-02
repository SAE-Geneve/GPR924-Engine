#include <gtest/gtest.h>
#include <numbers>
#include "maths/angle.h"

using namespace core::maths;

TEST(AngleConversionTest, DegreeToRadian)
{
  constexpr float deg90(90.0f);
  constexpr auto rad = Radian(deg90);
  EXPECT_NEAR(static_cast<float>(rad), std::numbers::pi_v<float> / 2.0f, 1e-5f);
}

TEST(AngleConversionTest, RadianToDegree)
{
  constexpr Radian rad_pi(std::numbers::pi_v<float>);
  const auto deg = Degree(rad_pi);
  EXPECT_NEAR(static_cast<float>(deg), 180.0f, 1e-5f);
}

TEST(AngleConversionTest, RoundTripConversion)
{
  constexpr float original(123.45f);
  constexpr Radian rad(original);
  const Degree converted(rad);
  EXPECT_NEAR(static_cast<float>(converted), original, 1e-5f);
}