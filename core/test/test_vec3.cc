#include <gtest/gtest.h>

#include "maths/vec3.h"

using namespace core::maths;


TEST(Vec3, Constructor)
{
  constexpr Vec3I vec_i(1, -2, 3);
  EXPECT_EQ(vec_i.x, 1);
  EXPECT_EQ(vec_i.y, -2);
  EXPECT_EQ(vec_i.z, 3);
  constexpr Vec3F vec_f(1.5f, 2.5f, 3.5f);
  EXPECT_FLOAT_EQ(vec_f.x, 1.5f);
  EXPECT_FLOAT_EQ(vec_f.y, 2.5f);
  EXPECT_FLOAT_EQ(vec_f.z, 3.5f);
  constexpr Vec3U vec_u(1, 2, 3);
  EXPECT_EQ(vec_u.x, 1);
  EXPECT_EQ(vec_u.y, 2);
  EXPECT_EQ(vec_u.z, 3);
}