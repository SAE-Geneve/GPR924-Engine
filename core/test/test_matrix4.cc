#include "gtest/gtest.h"
#include "maths/matrix4.h"

TEST(Matrix4DeterminantTest, IdentityMatrix) {
  std::array<std::array<float, 4>, 4> mat = {{
    {{1, 0, 0, 0}},
    {{0, 1, 0, 0}},
    {{0, 0, 1, 0}},
    {{0, 0, 0, 1}}
  }};
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
  EXPECT_FLOAT_EQ(m.determinant(), 120.0f);
}

TEST(Matrix4DeterminantTest, KnownMatrix) {
  std::array<std::array<size_t, 4>, 4> mat = {{
    {{3, 2, 0, 1}},
    {{4, 0, 1, 2}},
    {{3, 0, 2, 1}},
    {{9, 2, 3, 1}}
  }};
  core::maths::Matrix4<size_t> m(mat);
  EXPECT_EQ(m.determinant(), 24);
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
  for (size_t i = 0; i < 4; ++i)
    for (size_t j = 0; j < 4; ++j)
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
  for (size_t i = 0; i < 4; ++i)
    for (size_t j = 0; j < 4; ++j)
      EXPECT_NEAR(result.GetMatrix()[i][j], (i == j) ? 1.0f : 0.0f, 1e-5f);
}

TEST(Matrix4Test, TransposeWorks) {
  std::array<std::array<float, 4>, 4> arr = {{
    {{1, 2, 3, 4}},
    {{5, 6, 7, 8}},
    {{9, 10, 11, 12}},
    {{13, 14, 15, 16}}
  }};
  core::maths::Matrix4<float> mat(arr);
  auto transposed = mat.transpose();

  for (size_t i = 0; i < 4; ++i)
    for (size_t j = 0; j < 4; ++j)
      EXPECT_FLOAT_EQ(transposed.GetMatrix()[i][j], arr[j][i]);
}
