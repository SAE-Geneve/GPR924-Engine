#include "gtest/gtest.h"
#include "maths/matrix3.h"

TEST(Matrix3, Determinant) {
  constexpr  std::array<std::array<float, 3>, 3> matrix = {{
    {1, 2, 3},
    {4, 5, 6},
    {7, 8, 9}
  }};

  core::maths::Matrix3 m(matrix);
  EXPECT_EQ(m.Det(), 0);
}

TEST(Matrix3, Identity) {
  auto m = core::maths::Matrix3<int>::Identity();

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_EQ(m(i,j), i==j ? 1 : 0);
}