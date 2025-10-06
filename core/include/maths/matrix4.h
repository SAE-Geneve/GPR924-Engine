//
// Created by hugze on 06.10.2025.
//

#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>

namespace core::maths {
template <typename T>
requires std::is_arithmetic_v<T>
class Matrix4 {
public:
  explicit Matrix4(std::array<std::array<T, 4>, 4> newMatrix) {
    m = newMatrix;
  }

  // explicit Matrix4(Matrix4<T>& newMatrix) {
  //   m = newMatrix;
  // }

  explicit Matrix4() {
    m =  {{{{1, 0, 0, 0}}, {{0, 1, 0, 0}}, {{0, 0, 1, 0}}, {{0, 0, 0, 1}}}};
  }

  T determinant() const {
    T det = 0;

    // Laplace expansion on first row
    for (int col = 0; col < 4; ++col) {
      // Build 3x3 submatrix excluding row 0 and current column
      std::array<std::array<T, 3>, 3> sub{};
      for (int i = 1; i < 4; ++i) {
        int subCol = 0;
        for (int j = 0; j < 4; ++j) {
          if (j == col) continue;
          sub[i - 1][subCol] = m[i][j];
          ++subCol;
        }
      }

      // Compute determinant of the 3x3 submatrix
      T subDet =
          sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) -
          sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) +
          sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);

      // Add cofactor contribution
      T sign = (col % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
      det += sign * m[0][col] * subDet;
    }

    return det;
  }

  Matrix4 transpose() const {
    Matrix4<T> trans;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        trans.m[i][j] = m[j][i];
      }
    }
    return trans;
  }

  Matrix4<T> inverse() const {
    Matrix4<T> inv({{}});
    const auto& a = m;

    // Compute determinant first
    T det = determinant();
    if (det == static_cast<T>(0))
      throw std::runtime_error("Matrix not invertible");

    // Build the adjugate (transpose of cofactors)
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        // Build 3x3 minor
        std::array<std::array<T, 3>, 3> minor{};
        int rowIdx = 0;
        for (int r = 0; r < 4; r++) {
          if (r == i) continue;
          int colIdx = 0;
          for (int c = 0; c < 4; c++) {
            if (c == j) continue;
            minor[rowIdx][colIdx] = a[r][c];
            ++colIdx;
          }
          ++rowIdx;
        }

        // Determinant of 3x3 minor
        T minorDet =
            minor[0][0]*(minor[1][1]*minor[2][2]-minor[1][2]*minor[2][1]) -
            minor[0][1]*(minor[1][0]*minor[2][2]-minor[1][2]*minor[2][0]) +
            minor[0][2]*(minor[1][0]*minor[2][1]-minor[1][1]*minor[2][0]);

        inv.m[j][i] = ((i+j)%2==0 ? 1 : -1) * minorDet / det;
      }
    }
    return inv;
  }

  [[nodiscard]] constexpr Matrix4 operator+(const Matrix4 other) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
  }

  [[nodiscard]] constexpr Matrix4 operator-(const Matrix4 other) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
  }

  [[nodiscard]] constexpr Matrix4 operator*(const float& scalar) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; i++)
      for (int j = 0; j < 4; j++)
        result.m[i][j] = m[i][j] * scalar;
    return result;
  }

  [[nodiscard]] constexpr Matrix4 operator/(const float& scalar) const
  {
    Matrix4 result;
    for (int i = 0; i < 4; ++i) {
      for (int j = 0; j < 4; ++j) {
        result.m[i][j] = m[i][j] / scalar;
      }
    }
    return result;
  }

  [[nodiscard]] constexpr Matrix4 operator*(const Matrix4 other) const
  {
    Matrix4<T> result;
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        result.m[i][j] = 0;
        for (int k = 0; k < 4; k++)
          result.m[i][j] += m[i][k] * other.m[k][j];
      }
    }
    return result;
  }

  // [[nodiscard]] constexpr Matrix4 operator*(const Vec4& other) const
  // {
  //   return {};
  // }

  std::array<std::array<T, 4>, 4> GetMatrix() const { return m; }

private:
  std::array<std::array<T, 4>, 4> m;
};
}


#endif //MATRIX4_H
