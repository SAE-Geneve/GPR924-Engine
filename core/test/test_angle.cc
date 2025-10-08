//
// Contributors: Anthony Barman
//
#include <gtest/gtest.h>
#include <numbers>
#include "maths/angle.h"

TEST(AngleConversionTest, DegreeToRadian)
{
  constexpr core::Degree deg90(90.0f);
  const auto rad = core::Radian(deg90);
  EXPECT_NEAR(static_cast<float>(rad), std::numbers::pi_v<float> / 2.0f, 1e-5f);
}

TEST(AngleConversionTest, RadianToDegree)
{
  constexpr core::Radian rad_pi(std::numbers::pi_v<float>);
  const auto deg = core::Degree(rad_pi);
  EXPECT_NEAR(static_cast<float>(deg), 180.0f, 1e-5f);
}

TEST(AngleConversionTest, RoundTripConversion)
{
  constexpr core::Degree original(123.45f);
  const core::Radian rad(original);
  const core::Degree converted(rad);
  EXPECT_NEAR(static_cast<float>(converted), static_cast<float>(original), 1e-5f);
}