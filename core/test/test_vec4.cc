#include "maths/vec4.h"

#include <gtest/gtest.h>

using namespace core::maths;

TEST(Vec4, Constructor)
{
  constexpr Vec4I vec_i(1, -2, 3, -4);
  EXPECT_EQ(vec_i.x, 1);
  EXPECT_EQ(vec_i.y, -2);
  EXPECT_EQ(vec_i.z, 3);
  EXPECT_EQ(vec_i.w, -4);
  constexpr Vec4F vec_f(1.5f, 2.5f, 3.5f, 4.5f);
  EXPECT_FLOAT_EQ(vec_f.x, 1.5f);
  EXPECT_FLOAT_EQ(vec_f.y, 2.5f);
  EXPECT_FLOAT_EQ(vec_f.z, 3.5f);
  EXPECT_FLOAT_EQ(vec_f.w, 4.5f);
  constexpr Vec4U vec_u(1, 2, 3, 4);
  EXPECT_EQ(vec_u.x, 1);
  EXPECT_EQ(vec_u.y, 2);
  EXPECT_EQ(vec_u.z, 3);
  EXPECT_EQ(vec_u.w, 4);
}