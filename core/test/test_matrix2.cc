//
// Created by WIXXEL on 07.10.2025.
//

#include "gtest/gtest.h"
#include "maths/matrix2.h"

TEST(Matrix2, Determinant) {
  constexpr std::array<float, 4> matrix = {
    1, 2,
    3, 4
  };

  constexpr core::maths::Matrix2 m(matrix);
  EXPECT_EQ(m.Det(), -2);
}

TEST(Matrix2, ZeroMatrix) {
  constexpr core::maths::Matrix2<int> m;
  EXPECT_EQ(m.Det(), 0);
}

TEST(Matrix2, Transpose) {
  constexpr std::array<float, 4> matrix = {
    1, 2,
    3, 4
  };

  constexpr core::maths::Matrix2 m(matrix);
  const auto t = m.Transpose();

  EXPECT_EQ(t(0,1), m(1,0));
  EXPECT_NE(t(0,1), m(0,1));
  EXPECT_EQ(m.Det(), t.Det());
}

TEST(Matrix2, Identity) {
  constexpr auto m = core::maths::Matrix2<int>::Identity();

  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      EXPECT_EQ(m(i,j), i==j ? 1 : 0);
}

TEST(Matrix2, InverseDeterminantToZero) {
  constexpr core::maths::Matrix2<int> m;
  EXPECT_THROW((void)m.Inverse();, std::domain_error);
}

TEST(Matrix2, InverseOfIdentityIsIdentity) {
  constexpr auto m = core::maths::Matrix2<int>::Identity();

  const auto inv = m.Inverse();
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 2; ++j)
      EXPECT_EQ(m(i, j), inv(i, j));
}

TEST(Matrix2, Multiplication) {
  constexpr std::array<float, 4> a = {
    1, 2,
    3, 4
  };
  constexpr std::array<float, 4> b = {
    2, 0,
    1, 2
  };

  constexpr std::array<float, 4> expected = {
    4, 4,
    10, 8
  };

  const core::maths::Matrix2 A(a);
  const core::maths::Matrix2 B(b);
  const core::maths::Matrix2 result = A * B;
  constexpr core::maths::Matrix2 E(expected);

  for (size_t i = 0; i < 2; ++i)
    for (size_t j = 0; j < 2; ++j)
      EXPECT_EQ(result(i, j), E(i, j));
}

TEST(Matrix2, Addition) {
  constexpr std::array<float, 4> a = {
    1, 2,
    3, 4
  };
  constexpr std::array<float, 4> b = {
    4, 3,
    2, 1
  };

  const core::maths::Matrix2 A(a);
  const core::maths::Matrix2 B(b);
  const core::maths::Matrix2 result = A + B;

  for (size_t i = 0; i < 2; ++i)
    for (size_t j = 0; j < 2; ++j)
      EXPECT_EQ(result(i, j), 5);
}

TEST(Matrix2, Subtraction) {
  constexpr std::array<float, 4> a = {
    1, 2,
    3, 4
  };
  constexpr std::array<float, 4> b = {
    4, 3,
    2, 1
  };

  constexpr std::array<float, 4> expected = {
    1-4, 2-3,
    3-2, 4-1
  };

  const core::maths::Matrix2 A(a);
  const core::maths::Matrix2 B(b);
  const core::maths::Matrix2 result = A - B;
  constexpr core::maths::Matrix2 E(expected);

  for (size_t i = 0; i < 2; ++i)
    for (size_t j = 0; j < 2; ++j)
      EXPECT_EQ(result(i, j), E(i, j));
}
