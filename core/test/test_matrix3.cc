#include "gtest/gtest.h"
#include "maths/matrix3.h"

TEST(Matrix3, Determinant) {
  constexpr std::array<float, 9> matrix ={
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  };

  constexpr core::maths::Matrix3 m(matrix);
  EXPECT_EQ(m.Det(), 0);
}

TEST(Matrix3, ZeroMatrix) {
  constexpr core::maths::Matrix3<int> m;
  EXPECT_EQ(m.Det(), 0);
}

TEST(Matrix3, Transpose) {
  constexpr std::array<float, 9> matrix ={
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
  };

  constexpr core::maths::Matrix3 m(matrix);
  const auto t = m.Transpose();

  EXPECT_EQ(t(0,1), m(1,0));
  EXPECT_NE(t(0,1), m(0,1));
  EXPECT_EQ(m.Det(), t.Det());
}

TEST(Matrix3, Identity) {
  constexpr auto m = core::maths::Matrix3<int>::Identity();

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_EQ(m(i,j), i==j ? 1 : 0);
}