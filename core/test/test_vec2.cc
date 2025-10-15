#include <gtest/gtest.h>

#include "maths/vec2.h"

TEST(Vec2, Constructor)
{
  constexpr core::Vec2I vec_i(1, -3);
  EXPECT_EQ(vec_i.x, 1);
  EXPECT_EQ(vec_i.y, -3);

  constexpr core::Vec2F vec_f(1.5f, 2.5f);
  EXPECT_FLOAT_EQ(vec_f.x, 1.5f);
  EXPECT_FLOAT_EQ(vec_f.y, 2.5f);

  constexpr core::Vec2U vec_u(1, 6);
  EXPECT_EQ(vec_u.x, 1);
  EXPECT_EQ(vec_u.y, 6);
}

TEST(Vec2, Add)
{
  constexpr core::Vec2I vec1(6, 3);
  constexpr core::Vec2I vec2(4, 5);
  constexpr core::Vec2I vec3 = vec1 + vec2;
  core::Vec2I vec4(6, 3);
  vec4 += vec2 * 2;
  EXPECT_EQ(vec3.x, 10);
  EXPECT_EQ(vec3.y, 8);
  EXPECT_EQ(vec4.x, 14);
  EXPECT_EQ(vec4.y, 13);

  constexpr core::Vec2F vec_f1(1.2f, 2.2f);
  constexpr core::Vec2F vec_f2(4.3f, 5.3f);
  constexpr core::Vec2F vec_f3 = vec_f1 + vec_f2;
  core::Vec2F vec_4f(1.2f, 2.2f);
  vec_4f += vec_f2 * 2.f;
  EXPECT_FLOAT_EQ(vec_f3.x, 5.5f);
  EXPECT_FLOAT_EQ(vec_f3.y,  7.5f);
  EXPECT_EQ(vec_4f.x, 9.8f);
  EXPECT_EQ(vec_4f.y, 12.8f);
}

TEST(Vec2, Sub)
{
  constexpr core::Vec2I vec1(4, 5);
  constexpr core::Vec2I vec2(1, 2);
  constexpr core::Vec2I vec3 = vec1 - vec2;
  core::Vec2I vec4(5, 6);
  vec4 -= vec1;
  EXPECT_EQ(vec3.x, 3);
  EXPECT_EQ(vec3.y, 3);
  EXPECT_EQ(vec4.x, 1);
  EXPECT_EQ(vec4.y, 1);

  constexpr core::Vec2F vec_f1(4.5f, 5.5f);
  constexpr core::Vec2F vec_f2(1.2f, 2.2f);
  constexpr core::Vec2F vec_f3 = vec_f1 - vec_f2;
  core::Vec2F vec_f4(5.5f, 6.5f);
  vec_f4 -= vec_f1;
  EXPECT_FLOAT_EQ(vec_f3.x,  3.3f);
  EXPECT_FLOAT_EQ(vec_f3.y,  3.3f);
  EXPECT_FLOAT_EQ(vec_f4.x,  1.f);
  EXPECT_FLOAT_EQ(vec_f4.y,  1.f);

}

TEST(Vec2, Muli)
{
  constexpr core::Vec2I vec1(4, 5);

  constexpr auto result_int = vec1 * 2;
  EXPECT_EQ(result_int.x, 4*2);
  EXPECT_EQ(result_int.y, 5*2);

  constexpr core::Vec2I vec2(1, 2);
  constexpr core::Vec2I vec12 = vec1 * vec2;
  EXPECT_EQ(vec12.x, 1*4);
  EXPECT_EQ(vec12.y, 2*5);


  constexpr core::Vec2I vec3 = 2 * vec2;
  EXPECT_EQ(vec3.x, 1 * 2);
  EXPECT_EQ(vec3.y, 2 * 2);

  core::Vec2I vec4(2, 6);
  vec4 *= 2;
  EXPECT_EQ(vec4.x, 2*2);
  EXPECT_EQ(vec4.y, 6*2);

  constexpr core::Vec2F vec_f1(4.3f, 5.3f);

  constexpr auto result_float = vec_f1 * 2;
  EXPECT_FLOAT_EQ(result_float.x, 4.3f*2);
  EXPECT_FLOAT_EQ(result_float.y, 5.3f*2);

  constexpr core::Vec2F vec_f2(1.2f, 2.2f);
  constexpr core::Vec2F vec_f12 = vec_f1 * vec_f2;
  EXPECT_FLOAT_EQ(vec_f12.x, 4.3f * 1.2f);
  EXPECT_FLOAT_EQ(vec_f12.y, 5.3f * 2.2f);


  constexpr core::Vec2F vec_f3 = 2 * vec_f2;
  EXPECT_FLOAT_EQ(vec_f3.x, 1.2f * 2);
  EXPECT_FLOAT_EQ(vec_f3.y, 2.2f * 2);
}

TEST(Vec2, Equal)
{
  constexpr core::Vec2I vec1(4, 5);
  constexpr core::Vec2I vec2(1, 2);
  constexpr core::Vec2I vec3(4, 5);
  constexpr core::Vec2F vec_f1(4.5f, 5.5f);
  constexpr core::Vec2F vec_f2(1.2f, 2.2f);
  constexpr core::Vec2F vec_f3(4.5f, 5.5f);

  EXPECT_TRUE(vec1 == vec3);
  EXPECT_FALSE(vec1 == vec2);

  EXPECT_TRUE(vec_f1 == vec_f3);
  EXPECT_FALSE(vec_f1 == vec_f2);
}

TEST(Vec2, Dot)
{
  constexpr core::Vec2I vec1(4, 5);
  constexpr core::Vec2I vec2(1, 2);
  EXPECT_EQ(vec1.Dot(vec2), 4 * 1 + 5 * 2);
  EXPECT_EQ(core::Vec2I::Dot(vec1, vec2), 4 * 1 + 5 * 2);

  constexpr core::Vec2F vec_f1(4.3f, 5.3f);
  constexpr core::Vec2F vec_f2(1.2f, 2.2f);
  EXPECT_FLOAT_EQ(vec_f1.Dot(vec_f2), 4.3f * 1.2f + 5.3f * 2.2f);
  EXPECT_FLOAT_EQ(core::Vec2F::Dot(vec_f1, vec_f2), 4.3f * 1.2f + 5.3f * 2.2f);
}

TEST(Vec2, Cross)
{
  constexpr core::Vec2I vec1(4, 5);
  constexpr core::Vec2I vec2(1, 2);
  constexpr core::Vec2I vec12 = vec1.Cross(vec2);
  EXPECT_EQ(vec12.x, 5 * 1 - 4 * 2);
  EXPECT_EQ(vec12.y, 4 * 2 - 5 * 1);
  constexpr core::Vec2I vec21 = core::Vec2I::Cross(vec2, vec1);
  EXPECT_EQ(vec21.x, 4 * 2 - 1 * 5);
  EXPECT_EQ(vec21.y,  1 * 5 - 2 * 4);


  constexpr core::Vec2F vec_f1(4.3f, 5.3f);
  constexpr core::Vec2F vec_f2(1.2f, 2.2f);
  constexpr core::Vec2F vec_f12 = vec_f1.Cross(vec_f2);
  EXPECT_FLOAT_EQ(vec_f12.x, 5.3f * 1.2f - 4.3f * 2.2f);
  EXPECT_FLOAT_EQ(vec_f12.y, 4.3f * 2.2f - 5.3f * 1.2f);
  constexpr core::Vec2F vec_f21 = core::Vec2F::Cross(vec_f2, vec_f1);
  EXPECT_FLOAT_EQ(vec_f21.x, 2.2f * 4.3f - 1.2f * 5.3f);
  EXPECT_FLOAT_EQ(vec_f21.y, 1.2f * 5.3f - 2.2f * 4.3f);
}

TEST(Vec2, Div)
{
  constexpr core::Vec2I vec1(4, 5);

  constexpr auto result_int = vec1 / 2;
  EXPECT_EQ(result_int.x, 4/2);
  EXPECT_EQ(result_int.y, 5/2);

  constexpr core::Vec2I vec2(1, 2);
  constexpr core::Vec2I vec3 = vec1 / vec2;
  EXPECT_EQ(vec3.x, 4/1);
  EXPECT_EQ(vec3.y, 5/2);

  constexpr core::Vec2F vec_f1(4.3f, 5.3f);

  constexpr auto result_float = vec_f1 / 2;
  EXPECT_FLOAT_EQ(result_float.x, 4.3f/2);
  EXPECT_FLOAT_EQ(result_float.y, 5.3f/2);

  constexpr core::Vec2F vec_f2(1.2f, 2.2f);
  constexpr core::Vec2F vec_f3 = vec_f1 / vec_f2;
  EXPECT_FLOAT_EQ(vec_f3.x, 4.3f/1.2f);
  EXPECT_FLOAT_EQ(vec_f3.y, 5.3f/2.2f);
}

TEST(Vec2, MagnitudeSqu)
{
  constexpr core::Vec2F vec_1(1.2f, 2.2f);
  const auto result_float = vec_1.magnitude_sqr();
  EXPECT_FLOAT_EQ(result_float, 1.2f * 1.2f + 2.2f * 2.2f);
}

TEST(Vec2, Magnitude)
{
  constexpr core::Vec2F vec_1(1.2f, 2.2f);
  const auto result_float = vec_1.magnitude();
  EXPECT_FLOAT_EQ(result_float, std::sqrt(vec_1.magnitude_sqr()));
}

TEST(Vec2, Normalize)
{
  constexpr core::Vec2F vec_1(1.2f, 2.2f);
  const auto result_float = vec_1.Normalize();

  const float result_test = vec_1.magnitude();
  core::Vec2F vec_1_test(0, 0);
  if (result_test != 0.0f)
  {
    vec_1_test = vec_1 / result_test;
  }

  EXPECT_FLOAT_EQ(result_float.x, vec_1_test.x);
  EXPECT_FLOAT_EQ(result_float.y, vec_1_test.y);
}

TEST(Vec2, Rotate)
{
  constexpr core::Vec2F test_vec(1.2f, 2.2f);
  auto result_float = test_vec.Rotate(core::Radian(core::Degree(90)));

  auto c = std::cos(
      static_cast<float>(core::Radian(core::Degree(90))));
  auto s = std::sin(
      static_cast<float>(core::Radian(core::Degree(90))));

  float new_x = test_vec.x * c - test_vec.y * s;
  float new_y = test_vec.x * s + test_vec.y * c;
  core::Vec2F result_test(new_x, new_y);

  EXPECT_FLOAT_EQ(result_float.x, result_test.x);
  EXPECT_FLOAT_EQ(result_float.y, result_test.y);;
}

TEST(Vec2, Lerp)
{
  constexpr core::Vec2F vec_1(1.2f, 2.2f);
  constexpr core::Vec2F vec_2(4.3f, 5.3f);
  constexpr auto result_ = core::Vec2F::Lerp(vec_1, vec_2, 0.5f);
  EXPECT_FLOAT_EQ(result_.x, 1.2f * (1 - 0.5f) + 4.3f * 0.5f);
  EXPECT_FLOAT_EQ(result_.y, 2.2f * (1 - 0.5f) + 5.3f * 0.5f);
}

TEST(Vec2, Slerp)
{
  constexpr core::Vec2F vec_1(1.2f, 2.2f);
  constexpr core::Vec2F vec_2(4.3f, 5.3f);
  const core::Vec2F n1 = vec_1.Normalize();
  const core::Vec2F n2 = vec_2.Normalize();
  const auto result_ = core::Vec2F::Slerp(n1, n2, 0.5f);

  const float dot = n1.Dot(n2);
  const float theta = std::acos(dot);
  const float sinTheta = std::sin(theta);

  const float w0 = std::sin((1 - 0.5f) * theta) / sinTheta;
  const float w1 = std::sin(0.5f * theta) / sinTheta;

  EXPECT_FLOAT_EQ(result_.x, n1.x * w0 + n2.x * w1);
  EXPECT_FLOAT_EQ(result_.y, n1.y * w0 + n2.y * w1);
}

TEST(Vec2, Projection)
{
  constexpr core::Vec2I vec1(1, 2);
  constexpr core::Vec2I vec2(4, 5);

  constexpr int factor = vec1.Dot(vec2) / vec2.Dot(vec2);

  constexpr core::Vec2I vec12 = core::Vec2I::Projection(vec1, vec2);
  constexpr core::Vec2I vec_test(4 * factor, 5 * factor);
  EXPECT_EQ(vec12.x, vec_test.x);
  EXPECT_EQ(vec12.y, vec_test.y);

  constexpr core::Vec2F vec_f1(1.2f, 2.2f);
  constexpr core::Vec2F vec_f2(4.3f, 5.3f);

  constexpr float factor_f = vec_f1.Dot(vec_f2) / vec_f2.Dot(vec_f2);

  constexpr core::Vec2F vec_f12 = core::Vec2F::Projection(vec_f1, vec_f2);
  constexpr core::Vec2F vec__f_test(4.3f * factor_f, 5.3f * factor_f);
  EXPECT_EQ(vec_f12.x, vec__f_test.x);
  EXPECT_EQ(vec_f12.y, vec__f_test.y);
}

TEST(Vec2, Reflection)
{
  constexpr core::Vec2I vec1(1, 2);
  constexpr core::Vec2I vec2(4, 5);

  constexpr int factor = 2 * vec1.Dot(vec2);

  constexpr core::Vec2I vec12 = core::Vec2I::Reflection(vec1, vec2);
  constexpr core::Vec2I vec_test(1 - 4 * factor, 2 - 5 * factor);
  EXPECT_EQ(vec12.x, vec_test.x);
  EXPECT_EQ(vec12.y, vec_test.y);

  constexpr core::Vec2F vec_f1(1.2f, 2.2f);
  constexpr core::Vec2F vec_f2(4.3f, 5.3f);

  constexpr float factor_f = 2 * vec_f1.Dot(vec_f2);

  constexpr core::Vec2F vec_f12 = core::Vec2F::Reflection(vec_f1, vec_f2);
  constexpr core::Vec2F vec_f_test(1.2f - 4.3f * factor_f, 2.2f - 5.3f * factor_f);
  EXPECT_EQ(vec_f12.x, vec_f_test.x);
  EXPECT_EQ(vec_f12.y, vec_f_test.y);
}

TEST(Vec2, Perpendicular)
{
  constexpr core::Vec2F test_vec(1.2f, 2.2f);

  auto result_float_1 = test_vec.PerpendicularClockWise();
  core::Vec2F result_test_1(test_vec.y, -test_vec.x);;
  EXPECT_FLOAT_EQ(result_float_1.x, result_test_1.x);
  EXPECT_FLOAT_EQ(result_float_1.y, result_test_1.y);

  auto result_float_2 = test_vec.PerpendicularCounterClockWise();
  core::Vec2F result_test_2(-test_vec.y, test_vec.x);;
  EXPECT_FLOAT_EQ(result_float_2.x, result_test_2.x);
  EXPECT_FLOAT_EQ(result_float_2.y, result_test_2.y);
}