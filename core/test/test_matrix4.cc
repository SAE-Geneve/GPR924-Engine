#include "gtest/gtest.h"
#include "maths/matrix4.h"

TEST(Matrix4DeterminantTest, IdentityMatrix) {
  std::array<std::array<float, 4>, 4> mat = {
    {{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{0, 0, 0, 1}}}};
  core::maths::Matrix4<float> m(mat);
  EXPECT_FLOAT_EQ(m.determinant(), 1.0f);
}

TEST(Matrix4DeterminantTest, ZeroMatrix) {
  std::array<std::array<float, 4>, 4> mat = {{
    {{0, 0, 0, 0}},
    {{0, 0, 0, 0}},
    {{0, 0, 0, 0}},
    {{0, 0, 0, 0}}
  }};
  core::maths::Matrix4<float> m(mat);
  EXPECT_FLOAT_EQ(m.determinant(), 0.0f);
}

TEST(Matrix4DeterminantTest, DiagonalMatrix) {
  std::array<std::array<float, 4>, 4> mat = {{
    {{2, 0, 0, 0}},
    {{0, 3, 0, 0}},
    {{0, 0, 4, 0}},
    {{0, 0, 0, 5}}
  }};
  core::maths::Matrix4<float> m(mat);
  EXPECT_FLOAT_EQ(m.determinant(), 2 * 3 * 4 * 5); // 120
}

TEST(Matrix4DeterminantTest, UpperTriangularMatrix) {
  std::array<std::array<double, 4>, 4> mat = {{
    {{2, 3, 4, 5}},
    {{0, 6, 7, 8}},
    {{0, 0, 9, 10}},
    {{0, 0, 0, 11}}
  }};
  core::maths::Matrix4<double> m(mat);
  EXPECT_DOUBLE_EQ(m.determinant(), 2 * 6 * 9 * 11); // 1188
}

TEST(Matrix4DeterminantTest, KnownMatrix) {
  std::array<std::array<int, 4>, 4> mat = {{
    {{3, 2, 0, 1}},
    {{4, 0, 1, 2}},
    {{3, 0, 2, 1}},
    {{9, 2, 3, 1}}
  }};
  core::maths::Matrix4<int> m(mat);
  EXPECT_EQ(m.determinant(), 24);
}

TEST(Matrix4DeterminantTest, SingularMatrix) {
  std::array<std::array<float, 4>, 4> mat = {{
    {{1, 2, 3, 4}},
    {{5, 6, 7, 8}},
    {{9, 10, 11, 12}},
    {{13, 14, 15, 16}}
  }};
  core::maths::Matrix4<float> m(mat);
  EXPECT_FLOAT_EQ(m.determinant(), 0.0f);
}


TEST(Matrix4OperatorsTest, Addition) {
    std::array<std::array<float, 4>, 4> a = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};

    std::array<std::array<float, 4>, 4> b = {{
        {{1, 1, 1, 1}},
        {{1, 1, 1, 1}},
        {{1, 1, 1, 1}},
        {{1, 1, 1, 1}}
    }};

    core::maths::Matrix4<float> m1(a);
    core::maths::Matrix4<float> m2(b);
    auto result = m1 + m2;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(result.GetMatrix()[i][j], a[i][j] + b[i][j]);
}

TEST(Matrix4OperatorsTest, Subtraction) {
    std::array<std::array<float, 4>, 4> a = {{
        {{2, 2, 2, 2}},
        {{2, 2, 2, 2}},
        {{2, 2, 2, 2}},
        {{2, 2, 2, 2}}
    }};

    std::array<std::array<float, 4>, 4> b = {{
        {{1, 1, 1, 1}},
        {{1, 1, 1, 1}},
        {{1, 1, 1, 1}},
        {{1, 1, 1, 1}}
    }};

    core::maths::Matrix4<float> m1(a);
    core::maths::Matrix4<float> m2(b);
    auto result = m1 - m2;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(result.GetMatrix()[i][j], 1.0f);
}

TEST(Matrix4OperatorsTest, ScalarMultiplication) {
    std::array<std::array<float, 4>, 4> a = {{
        {{1, 2, 3, 4}},
        {{5, 6, 7, 8}},
        {{9, 10, 11, 12}},
        {{13, 14, 15, 16}}
    }};
    core::maths::Matrix4<float> m(a);
    auto result = m * 2.0f;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(result.GetMatrix()[i][j], a[i][j] * 2.0f);
}

TEST(Matrix4OperatorsTest, ScalarDivision) {
    std::array<std::array<float, 4>, 4> a = {{
        {{2, 4, 6, 8}},
        {{10, 12, 14, 16}},
        {{18, 20, 22, 24}},
        {{26, 28, 30, 32}}
    }};
    core::maths::Matrix4<float> m(a);
    auto result = m / 2.0f;

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(result.GetMatrix()[i][j], a[i][j] / 2.0f);
}

TEST(Matrix4OperatorsTest, MatrixMultiplication) {
    std::array<std::array<float, 4>, 4> a = {{
        {{1, 0, 0, 0}},
        {{0, 1, 0, 0}},
        {{0, 0, 1, 0}},
        {{0, 0, 0, 1}}
    }};
    std::array<std::array<float, 4>, 4> b = {{
        {{2, 3, 4, 5}},
        {{6, 7, 8, 9}},
        {{10, 11, 12, 13}},
        {{14, 15, 16, 17}}
    }};
    core::maths::Matrix4<float> m1(a);
    core::maths::Matrix4<float> m2(b);
    auto result = m1 * m2;

    // multiplying by identity should yield same matrix
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_FLOAT_EQ(result.GetMatrix()[i][j], b[i][j]);
}

TEST(Matrix4Test, InverseOfIdentityIsIdentity) {
  std::array<std::array<float, 4>, 4> identity = {{
    {{1, 0, 0, 0}},
    {{0, 1, 0, 0}},
    {{0, 0, 1, 0}},
    {{0, 0, 0, 1}}
  }};
  core::maths::Matrix4<float> mat(identity);
  auto inv = mat.inverse();

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_FLOAT_EQ(inv.GetMatrix()[i][j], identity[i][j]);
}

TEST(Matrix4Test, InverseMultipliedByOriginalGivesIdentity) {
  std::array<std::array<float, 4>, 4> data = {{
    {{4, 7, 2, 0}},
    {{3, 6, 1, 0}},
    {{2, 5, 1, 0}},
    {{0, 0, 0, 1}}
  }};
  core::maths::Matrix4<float> mat(data);
  auto inv = mat.inverse();
  auto result = mat * inv;

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_NEAR(result.GetMatrix()[i][j], (i == j) ? 1.0f : 0.0f, 1e-5);
}

using core::maths::Matrix4;

TEST(Matrix4Test, TransposeWorks) {
  std::array<std::array<float, 4>, 4> arr = {{
    {{1, 2, 3, 4}},
    {{5, 6, 7, 8}},
    {{9, 10, 11, 12}},
    {{13, 14, 15, 16}}
  }};
  Matrix4<float> mat(arr);
  auto transposed = mat.transpose();

  for (int i = 0; i < 4; ++i)
    for (int j = 0; j < 4; ++j)
      EXPECT_FLOAT_EQ(transposed.GetMatrix()[i][j], arr[j][i]);
}