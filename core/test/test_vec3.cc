//
// Contributors: Anthony Barman
//
#include <gtest/gtest.h>

#include "maths/vec3.h"

TEST(Vec3, Constructor)
{
  constexpr core::maths::Vec3I vec_i(1, -2, 3);
  EXPECT_EQ(vec_i.x, 1);
  EXPECT_EQ(vec_i.y, -2);
  EXPECT_EQ(vec_i.z, 3);
  constexpr core::maths::Vec3F vec_f(1.5f, 2.5f, 3.5f);
  EXPECT_FLOAT_EQ(vec_f.x, 1.5f);
  EXPECT_FLOAT_EQ(vec_f.y, 2.5f);
  EXPECT_FLOAT_EQ(vec_f.z, 3.5f);
  constexpr core::maths::Vec3U vec_u(1, 2, 3);
  EXPECT_EQ(vec_u.x, 1);
  EXPECT_EQ(vec_u.y, 2);
  EXPECT_EQ(vec_u.z, 3);
}

TEST(Vec3, add)
{
  constexpr core::maths::Vec3I vec1(1, 2, 3);
  constexpr core::maths::Vec3I vec2(4, 5, 6);
  constexpr core::maths::Vec3I vec3 = vec1 + vec2;
  EXPECT_EQ(vec3.x, 5);
  EXPECT_EQ(vec3.y, 7);
  EXPECT_EQ(vec3.z, 9);
  constexpr core::maths::Vec3F vec_f1(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f2(4.3f, 5.3f, 6.3f);
  constexpr core::maths::Vec3F vec_f3 = vec_f1 + vec_f2;
  EXPECT_FLOAT_EQ(vec_f3.x, 5.5f);
  EXPECT_FLOAT_EQ(vec_f3.y,  7.5f);
  EXPECT_FLOAT_EQ(vec_f3.z,  9.5f);
}

TEST(Vec3, Sub)
{
  constexpr core::maths::Vec3I vec1(4, 5, 6);
  constexpr core::maths::Vec3I vec2(1, 2, 3);
  constexpr core::maths::Vec3I vec3 = vec1 - vec2;
  EXPECT_EQ(vec3.x, 3);
  EXPECT_EQ(vec3.y, 3);
  EXPECT_EQ(vec3.z, 3);
  constexpr core::maths::Vec3F vec_f1(4.5f, 5.5f, 6.5f);
  constexpr core::maths::Vec3F vec_f2(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f3 = vec_f1 - vec_f2;
  EXPECT_FLOAT_EQ(vec_f3.x,  3.3f);
  EXPECT_FLOAT_EQ(vec_f3.y,  3.3f);
  EXPECT_FLOAT_EQ(vec_f3.z,  3.3f);
}

TEST(Vec3, Muli)
{
  constexpr core::maths::Vec3I vec1(4, 5, 6);

  constexpr auto result_int = vec1 * 2;
  EXPECT_EQ(result_int.x, 4*2);
  EXPECT_EQ(result_int.y, 5*2);
  EXPECT_EQ(result_int.z, 6*2);

  constexpr core::maths::Vec3I vec2(1, 2, 3);
  constexpr core::maths::Vec3I vec12 = vec1 * vec2;
  EXPECT_EQ(vec12.x, 1*4);
  EXPECT_EQ(vec12.y, 2*5);
  EXPECT_EQ(vec12.z, 3*6);


  constexpr core::maths::Vec3I vec3 = 2 * vec2;
  EXPECT_EQ(vec3.x, 1 * 2);
  EXPECT_EQ(vec3.y, 2 * 2);
  EXPECT_EQ(vec3.z, 3 * 2);


  constexpr core::maths::Vec3F vec_f1(4.3f, 5.3f, 6.3f);

  constexpr auto result_float = vec_f1 * 2;
  EXPECT_FLOAT_EQ(result_float.x, 4.3f*2);
  EXPECT_FLOAT_EQ(result_float.y, 5.3f*2);
  EXPECT_FLOAT_EQ(result_float.z, 6.3f*2);

  constexpr core::maths::Vec3F vec_f2(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f12 = vec_f1 * vec_f2;
  EXPECT_FLOAT_EQ(vec_f12.x, 4.3f * 1.2f);
  EXPECT_FLOAT_EQ(vec_f12.y, 5.3f * 2.2f);
  EXPECT_FLOAT_EQ(vec_f12.z, 6.3f * 3.2f);


  constexpr core::maths::Vec3F vec_f3 = 2 * vec_f2;
  EXPECT_FLOAT_EQ(vec_f3.x, 1.2f * 2);
  EXPECT_FLOAT_EQ(vec_f3.y, 2.2f * 2);
  EXPECT_FLOAT_EQ(vec_f3.z, 3.2f * 2);
}

TEST(Vec3, Dot)
{
  constexpr core::maths::Vec3I vec1(4, 5, 6);
  constexpr core::maths::Vec3I vec2(1, 2, 3);
  EXPECT_EQ(vec1.Dot(vec2), 4 * 1 + 5 * 2 + 6 * 3);
  EXPECT_EQ(core::maths::Vec3I::Dot(vec1, vec2), 4 * 1 + 5 * 2 + 6 * 3);

  constexpr core::maths::Vec3F vec_f1(4.3f, 5.3f, 6.3f);
  constexpr core::maths::Vec3F vec_f2(1.2f, 2.2f, 3.2f);
  EXPECT_FLOAT_EQ(vec_f1.Dot(vec_f2), 4.3f * 1.2f + 5.3f * 2.2f + 6.3f * 3.2f);
  EXPECT_FLOAT_EQ(core::maths::Vec3F::Dot(vec_f1, vec_f2), 4.3f * 1.2f + 5.3f * 2.2f + 6.3f * 3.2f);
}
//Test dot of result with vec1 and vec2 is 0/constant epsilon
TEST(Vec3, Cross)
{
  constexpr core::maths::Vec3I vec1(4, 5, 6);
  constexpr core::maths::Vec3I vec2(1, 2, 3);
  constexpr core::maths::Vec3I vec12 = vec1.Cross(vec2);
  EXPECT_EQ(vec12.x, 5 * 3 - 6 * 2);
  EXPECT_EQ(vec12.y, 6 * 1 - 4 * 3);
  EXPECT_EQ(vec12.z, 4 * 2 - 5 * 1);
  constexpr core::maths::Vec3I vec21 = core::maths::Vec3I::Cross(vec2, vec1);
  EXPECT_EQ(vec21.x, 2 * 6 - 3 * 5);
  EXPECT_EQ(vec21.y,  3 * 4 - 1 * 6);
  EXPECT_EQ(vec21.z, 1 * 5 - 2 * 4);


  constexpr core::maths::Vec3F vec_f1(4.3f, 5.3f, 6.3f);
  constexpr core::maths::Vec3F vec_f2(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f12 = vec_f1.Cross(vec_f2);
  EXPECT_FLOAT_EQ(vec_f12.x, 5.3f * 3.2f - 6.3f * 2.2f);
  EXPECT_FLOAT_EQ(vec_f12.y, 6.3f * 1.2f - 4.3f * 3.2f);
  EXPECT_FLOAT_EQ(vec_f12.z, 4.3f * 2.2f - 5.3f * 1.2f);
  constexpr core::maths::Vec3F vec_f21 = core::maths::Vec3F::Cross(vec_f2, vec_f1);
  EXPECT_FLOAT_EQ(vec_f21.x, 2.2f * 6.3f - 3.2f * 5.3f);
  EXPECT_FLOAT_EQ(vec_f21.y,  3.2f * 4.3f - 1.2f * 6.3f);
  EXPECT_FLOAT_EQ(vec_f21.z, 1.2f * 5.3f - 2.2f * 4.3f);
}

TEST(Vec3, Div)
{
  constexpr core::maths::Vec3I vec1(4, 5, 6);

  constexpr auto result_int = vec1 / 2;
  EXPECT_EQ(result_int.x, 4/2);
  EXPECT_EQ(result_int.y, 5/2);
  EXPECT_EQ(result_int.z, 6/2);

  constexpr core::maths::Vec3I vec2(1, 2, 3);
  constexpr core::maths::Vec3I vec3 = vec1 / vec2;
  EXPECT_EQ(vec3.x, 4/1);
  EXPECT_EQ(vec3.y, 5/2);
  EXPECT_EQ(vec3.z, 6/3);

  constexpr core::maths::Vec3F vec_f1(4.3f, 5.3f, 6.3f);

  constexpr auto result_float = vec_f1 / 2;
  EXPECT_FLOAT_EQ(result_float.x, 4.3f/2);
  EXPECT_FLOAT_EQ(result_float.y, 5.3f/2);
  EXPECT_FLOAT_EQ(result_float.z, 6.3f/2);

  constexpr core::maths::Vec3F vec_f2(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f3 = vec_f1 / vec_f2;
  EXPECT_FLOAT_EQ(vec_f3.x, 4.3f/1.2f);
  EXPECT_FLOAT_EQ(vec_f3.y, 5.3f/2.2f);
  EXPECT_FLOAT_EQ(vec_f3.z, 6.3f/3.2f);
}

TEST(Vec3, MagnitudeSqu)
{
  constexpr core::maths::Vec3F vec_1(1.2f, 2.2f, 3.2f);
  const auto result_float = vec_1.MagnitudeSqr();
  EXPECT_FLOAT_EQ(result_float, 1.2f * 1.2f + 2.2f * 2.2f + 3.2f * 3.2f);
}

TEST(Vec3, Magnitude)
{
  constexpr core::maths::Vec3F vec_1(1.2f, 2.2f, 3.2f);
  const auto result_float = vec_1.Magnitude();
  EXPECT_FLOAT_EQ(result_float, std::sqrt(vec_1.MagnitudeSqr()));
}

TEST(Vec3, Normalize)
{
  constexpr core::maths::Vec3F vec_1(1.2f, 2.2f, 3.2f);
  const auto result_float = vec_1.Normalize();

  const float result_test = vec_1.Magnitude();
  core::maths::Vec3F vec_1_test(0, 0, 0);
  if (result_test != 0.0f)
  {
    vec_1_test = vec_1 / result_test;
  }

  EXPECT_FLOAT_EQ(result_float.x, vec_1_test.x);
  EXPECT_FLOAT_EQ(result_float.y, vec_1_test.y);
  EXPECT_FLOAT_EQ(result_float.z, vec_1_test.z);
}

TEST(Vec3, Rotate)
{
  constexpr core::maths::Vec3F vec_1(1.2f, 2.2f, 3.2f);
  auto result_float = vec_1.RotateX(core::maths::Radian(core::maths::Degree(90)));

  auto c = std::cos(
      static_cast<float>(core::maths::Radian(core::maths::Degree(90))));
  auto s = std::sin(
      static_cast<float>(core::maths::Radian(core::maths::Degree(90))));

  float new_x = vec_1.x;
  float new_y = vec_1.y * c - vec_1.z * s;
  float new_z = vec_1.y * s + vec_1.z * c;
  core::maths::Vec3F result_test(new_x, new_y, new_z);

  EXPECT_FLOAT_EQ(result_float.x, result_test.x);
  EXPECT_FLOAT_EQ(result_float.y, result_test.y);
  EXPECT_FLOAT_EQ(result_float.z, result_test.z);

  result_float = vec_1.RotateY(core::maths::Radian(core::maths::Degree(90)));

  c = std::cos(
      static_cast<float>(core::maths::Radian(core::maths::Degree(90))));
  s = std::sin(
      static_cast<float>(core::maths::Radian(core::maths::Degree(90))));

  new_x = vec_1.x * c + s * vec_1.z;
  new_y = vec_1.y;
  new_z = -vec_1.x * s + vec_1.z * c;
  result_test = {new_x, new_y, new_z};

  EXPECT_FLOAT_EQ(result_float.x, result_test.x);
  EXPECT_FLOAT_EQ(result_float.y, result_test.y);
  EXPECT_FLOAT_EQ(result_float.z, result_test.z);

  result_float = vec_1.RotateZ(core::maths::Radian(core::maths::Degree(90)));

  c = std::cos(
      static_cast<float>(core::maths::Radian(core::maths::Degree(90))));
  s = std::sin(
      static_cast<float>(core::maths::Radian(core::maths::Degree(90))));

  new_x = vec_1.x * c - vec_1.y * s;
  new_y = vec_1.x * s + vec_1.y * c;
  new_z = vec_1.z;
  result_test = {new_x, new_y, new_z};

  EXPECT_FLOAT_EQ(result_float.x, result_test.x);
  EXPECT_FLOAT_EQ(result_float.y, result_test.y);
  EXPECT_FLOAT_EQ(result_float.z, result_test.z);
}

TEST(Vec3, Lerp)
{
  constexpr core::maths::Vec3F vec_1(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_2(4.3f, 5.3f, 6.3f);
  constexpr auto result_ = core::maths::Vec3F::Lerp(vec_1, vec_2, 0.5f);
  EXPECT_FLOAT_EQ(result_.x, 1.2f * (1 - 0.5f) + 4.3f * 0.5f);
  EXPECT_FLOAT_EQ(result_.y, 2.2f * (1 - 0.5f) + 5.3f * 0.5f);
  EXPECT_FLOAT_EQ(result_.z, 3.2f * (1 - 0.5f) + 6.3f * 0.5f);
}

TEST(Vec3, Slerp)
{
  constexpr core::maths::Vec3F vec_1(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_2(4.3f, 5.3f, 6.3f);
  const core::maths::Vec3F n1 = vec_1.Normalize();
  const core::maths::Vec3F n2 = vec_2.Normalize();
  const auto result_ = core::maths::Vec3F::Slerp(n1, n2, 0.5f);

  const float dot = n1.Dot(n2);
  const float theta = std::acos(dot);
  const float sinTheta = std::sin(theta);

  const float w0 = std::sin((1 - 0.5f) * theta) / sinTheta;
  const float w1 = std::sin(0.5f * theta) / sinTheta;

  EXPECT_FLOAT_EQ(result_.x, n1.x * w0 + n2.x * w1);
  EXPECT_FLOAT_EQ(result_.y, n1.y * w0 + n2.y * w1);
  EXPECT_FLOAT_EQ(result_.z, n1.z * w0 + n2.z * w1);
}


TEST(Vec3, Projection)
{
  constexpr core::maths::Vec3I vec1(1, 2, 3);
  constexpr core::maths::Vec3I vec2(4, 5, 6);

  constexpr int factor = vec1.Dot(vec2) / vec2.Dot(vec2);

  constexpr core::maths::Vec3I vec12 = core::maths::Vec3I::Projection(vec1, vec2);
  constexpr core::maths::Vec3I vec_test(4 * factor, 5 * factor, 6 * factor);
  EXPECT_EQ(vec12.x, vec_test.x);
  EXPECT_EQ(vec12.y, vec_test.y);
  EXPECT_EQ(vec12.z, vec_test.z);

  constexpr core::maths::Vec3F vec_f1(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f2(4.3f, 5.3f, 6.3f);

  constexpr float factor_f = vec_f1.Dot(vec_f2) / vec_f2.Dot(vec_f2);

  constexpr core::maths::Vec3F vec_f12 = core::maths::Vec3F::Projection(vec_f1, vec_f2);
  constexpr core::maths::Vec3F vec__f_test(4.3f * factor_f, 5.3f * factor_f, 6.3f * factor_f);
  EXPECT_EQ(vec_f12.x, vec__f_test.x);
  EXPECT_EQ(vec_f12.y, vec__f_test.y);
  EXPECT_EQ(vec_f12.z, vec__f_test.z);
}

TEST(Vec3, Reflection)
{
  constexpr core::maths::Vec3I vec1(1, 2, 3);
  constexpr core::maths::Vec3I vec2(4, 5, 6);

  constexpr int factor = 2 * vec1.Dot(vec2);

  constexpr core::maths::Vec3I vec12 = core::maths::Vec3I::Reflection(vec1, vec2);
  constexpr core::maths::Vec3I vec_test(1 - 4 * factor, 2 - 5 * factor, 3 - 6 * factor);
  EXPECT_EQ(vec12.x, vec_test.x);
  EXPECT_EQ(vec12.y, vec_test.y);
  EXPECT_EQ(vec12.z, vec_test.z);

  constexpr core::maths::Vec3F vec_f1(1.2f, 2.2f, 3.2f);
  constexpr core::maths::Vec3F vec_f2(4.3f, 5.3f, 6.3f);

  constexpr float factor_f = 2 * vec_f1.Dot(vec_f2);

  constexpr core::maths::Vec3F vec_f12 = core::maths::Vec3F::Reflection(vec_f1, vec_f2);
  constexpr core::maths::Vec3F vec_f_test(1.2f - 4.3f * factor_f, 2.2f - 5.3f * factor_f, 3.2f - 6.3f * factor_f);
  EXPECT_EQ(vec_f12.x, vec_f_test.x);
  EXPECT_EQ(vec_f12.y, vec_f_test.y);
  EXPECT_EQ(vec_f12.z, vec_f_test.z);
}