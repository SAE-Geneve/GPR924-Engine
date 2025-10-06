//
// Created by hugze on 06.10.2025.
//

#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>
#include <stdexcept>
#include <type_traits>

namespace core::maths {

template <typename T>
class Matrix4 {
  static_assert(std::is_arithmetic<T>::value, "Matrix4 requires arithmetic type");

public:
  // Constructors
  explicit Matrix4(const std::array<std::array<T, 4>, 4>& newMatrix) noexcept : m(newMatrix) {}

  explicit Matrix4() noexcept {
    m = {{{{1, 0, 0, 0}},
          {{0, 1, 0, 0}},
          {{0, 0, 1, 0}},
          {{0, 0, 0, 1}}}};
  }

  // Determinant
  [[nodiscard]] T determinant() const noexcept {
    T det = 0;

    for (size_t col = 0; col < 4; ++col) {
      std::array<std::array<T, 3>, 3> sub{};
      for (size_t i = 1; i < 4; ++i) {
        size_t subCol = 0;
        for (size_t j = 0; j < 4; ++j) {
          if (j == col) continue;
          sub[i - 1][subCol] = m[i][j];
          ++subCol;
        }
      }

      const T subDet =
          sub[0][0] * (sub[1][1] * sub[2][2] - sub[1][2] * sub[2][1]) -
          sub[0][1] * (sub[1][0] * sub[2][2] - sub[1][2] * sub[2][0]) +
          sub[0][2] * (sub[1][0] * sub[2][1] - sub[1][1] * sub[2][0]);

      const T sign = (col % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
      det += sign * m[0][col] * subDet;
    }

    return det;
  }

  // Transpose
  [[nodiscard]] Matrix4<T> transpose() const noexcept {
    Matrix4<T> trans;
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 4; ++j)
        trans.m[i][j] = m[j][i];
    return trans;
  }

  // Inverse
  [[nodiscard]] Matrix4<T> inverse() const {
    const T det = determinant();
    if (det == static_cast<T>(0))
      throw std::runtime_error("Matrix not invertible (determinant = 0)");

    Matrix4<T> inv({{}});
    const auto& a = m;

    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        std::array<std::array<T, 3>, 3> minor{};
        size_t rowIdx = 0;
        for (size_t r = 0; r < 4; ++r) {
          if (r == i) continue;
          size_t colIdx = 0;
          for (size_t c = 0; c < 4; ++c) {
            if (c == j) continue;
            minor[rowIdx][colIdx] = a[r][c];
            ++colIdx;
          }
          ++rowIdx;
        }

        const T minorDet =
            minor[0][0] * (minor[1][1] * minor[2][2] - minor[1][2] * minor[2][1]) -
            minor[0][1] * (minor[1][0] * minor[2][2] - minor[1][2] * minor[2][0]) +
            minor[0][2] * (minor[1][0] * minor[2][1] - minor[1][1] * minor[2][0]);

        const T sign = ((i + j) % 2 == 0) ? static_cast<T>(1) : static_cast<T>(-1);
        inv.m[j][i] = (sign * minorDet) / det; // transpose of cofactor
      }
    }

    return inv;
  }

  // Operators
  [[nodiscard]] Matrix4<T> operator+(const Matrix4<T>& other) const noexcept {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 4; ++j)
        result.m[i][j] = m[i][j] + other.m[i][j];
    return result;
  }

  [[nodiscard]] Matrix4<T> operator-(const Matrix4<T>& other) const noexcept {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 4; ++j)
        result.m[i][j] = m[i][j] - other.m[i][j];
    return result;
  }

  [[nodiscard]] Matrix4<T> operator*(const T& scalar) const noexcept {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 4; ++j)
        result.m[i][j] = m[i][j] * scalar;
    return result;
  }

  [[nodiscard]] Matrix4<T> operator/(const T& scalar) const {
    if (scalar == static_cast<T>(0))
      throw std::runtime_error("Division by zero in Matrix4::operator/");

    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i)
      for (size_t j = 0; j < 4; ++j)
        result.m[i][j] = m[i][j] / scalar;
    return result;
  }

  [[nodiscard]] Matrix4<T> operator*(const Matrix4<T>& other) const noexcept {
    Matrix4<T> result;
    for (size_t i = 0; i < 4; ++i) {
      for (size_t j = 0; j < 4; ++j) {
        result.m[i][j] = static_cast<T>(0);
        for (size_t k = 0; k < 4; ++k)
          result.m[i][j] += m[i][k] * other.m[k][j];
      }
    }
    return result;
  }

  [[nodiscard]] const std::array<std::array<T, 4>, 4>& GetMatrix() const noexcept {
    return m;
  }

private:
  std::array<std::array<T, 4>, 4> m;
};

} // namespace core::maths

#endif // MATRIX4_H
