#include "gtest/gtest.h"
#include "maths/matrix3.h"
#include "maths/matrix4.h"

TEST(Matrix3Determinant, Matrix3) {
  constexpr  std::array<std::array<int, 3>, 3> matrix = {{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  }};

  core::maths::Matrix3 m(matrix);
  // EXPECT_EQ(m.Det(), 0);
  FAIL();
}


TEST(Matrix4Determinant, Matrix4) {
  constexpr  std::array<std::array<int, 4>, 4> matrix = {{
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 1, 2, 3},
    {4, 5, 6, 7}
  }};

  core::maths::Matrix4 m(matrix);
  EXPECT_EQ(m.Det(), 0);
}