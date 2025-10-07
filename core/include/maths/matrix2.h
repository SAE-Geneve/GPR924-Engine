//
// Created by WIXXEL on 07.10.2025.
//

#ifndef MATRIX2_H
#define MATRIX2_H

#include <array>
#include <stdexcept>
#include <type_traits>

namespace core::maths {
  template <typename T>
  class Matrix2 {
    static_assert(std::is_arithmetic_v<T>, "Matrix2 requires arithmetic value type");

  public:
    explicit constexpr Matrix2(const std::array<T, 4>& newMatrix) : matrix_(newMatrix) {}
    explicit constexpr Matrix2() noexcept : matrix_() {}

    [[nodiscard]] static constexpr Matrix2 Identity() noexcept {
      return Matrix2(std::array<T, 4>{
          T(1), T(0),
          T(0), T(1)
      });
    }

    [[nodiscard]] T Det() const noexcept {
      return matrix_[0] * matrix_[3] - matrix_[1] * matrix_[2];
    }

    [[nodiscard]] Matrix2 Transpose() const noexcept {
      Matrix2 result;
      for (std::size_t i = 0; i < 2; ++i) {
        for (std::size_t j = 0; j < 2; ++j) {
          result(i, j) = (*this)(j, i);
        }
      }
      return result;
    }

    [[nodiscard]] Matrix2 Inverse() const {
      const T det = Det();
      if (det == static_cast<T>(0))
        throw std::domain_error("Matrix not invertible (determinant = 0)");

      Matrix2 result;
      result(0, 0) =  matrix_[3];
      result(0, 1) = -matrix_[1];
      result(1, 0) = -matrix_[2];
      result(1, 1) =  matrix_[0];

      for (auto& val : result.matrix_)
        val /= det;

      return result;
    }

    template<typename I>
    T& operator()(I i, I j) {
      static_assert(std::is_integral_v<I>, "Indices must be integral");
      return matrix_[static_cast<size_t>(i) * 2 + static_cast<size_t>(j)];
    }

    template<typename I>
    const T& operator()(I i, I j) const {
      static_assert(std::is_integral_v<I>, "Indices must be integral");
      return matrix_[static_cast<size_t>(i) * 2 + static_cast<size_t>(j)];
    }

    [[nodiscard]] Matrix2 operator+(const Matrix2& other) const noexcept {
      Matrix2 result;
      for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
          result(i, j) = (*this)(i, j) + other(i, j);
      return result;
    }

    [[nodiscard]] Matrix2 operator-(const Matrix2& other) const noexcept {
      Matrix2 result;
      for (size_t i = 0; i < 2; ++i)
        for (size_t j = 0; j < 2; ++j)
          result(i, j) = (*this)(i, j) - other(i, j);
      return result;
    }

    [[nodiscard]] Matrix2 operator*(const Matrix2& other) const noexcept {
      Matrix2 result;
      for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
          for (size_t k = 0; k < 2; ++k)
            result(i, j) += (*this)(i, k) * other(k, j);
        }
      }
      return result;
    }

  private:
    std::array<T, 4> matrix_;
  };
}

#endif // MATRIX2_H
