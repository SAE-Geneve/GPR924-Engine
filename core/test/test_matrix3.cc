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

TEST(Matrix3, InverseOfIdentityIsIdentity) {
  constexpr auto m = core::maths::Matrix3<int>::Identity();

  const auto inv = m.Inverse();
  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_EQ(m(i, j), inv(i, j));
}

TEST(Matrix3, Multiplication) {
  constexpr std::array<float, 9> a = {
    1, 2, 3,
    4, 5, 6,
    7, 8, 9
};
  constexpr std::array<float, 9> b = {
    9, 8, 7,
    6, 5, 4,
    3, 2, 1
};

  constexpr core::maths::Matrix3 A(a);
  constexpr core::maths::Matrix3 B(b);

  constexpr std::array<float, 9> expected = {
    30, 24, 18,
    84, 69, 54,
    138, 114, 90
};

  for (int i = 0; i < 3; ++i)
    for (int j = 0; j < 3; ++j)
      EXPECT_EQ(expected[i*3 + j],
                A(i,0)*B(0,j) + A(i,1)*B(1,j) + A(i,2)*B(2,j));
}