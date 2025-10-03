#include <gtest/gtest.h>
#include <numbers>
#include "maths/angle.h"

TEST(AngleConversionTest, DegreeToRadian)
{
  constexpr core::maths::Degree deg90(90.0f);
  const auto rad = core::maths::Radian(deg90);
  EXPECT_NEAR(static_cast<float>(rad), std::numbers::pi_v<float> / 2.0f, 1e-5f);
}

TEST(AngleConversionTest, RadianToDegree)
{
  constexpr core::maths::Radian rad_pi(std::numbers::pi_v<float>);
  const auto deg = core::maths::Degree(rad_pi);
  EXPECT_NEAR(static_cast<float>(deg), 180.0f, 1e-5f);
}

TEST(AngleConversionTest, RoundTripConversion)
{
  constexpr core::maths::Degree original(123.45f);
  const core::maths::Radian rad(original);
  const core::maths::Degree converted(rad);
  EXPECT_NEAR(static_cast<float>(converted), static_cast<float>(original), 1e-5f);
}