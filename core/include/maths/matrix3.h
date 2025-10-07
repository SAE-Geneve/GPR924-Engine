#ifndef MATRIX3_H
#define MATRIX3_H
#include <array>

namespace core::maths {
  template <typename T>
  class Matrix3 {
    static_assert(std::is_arithmetic_v<T>, "Matrix3 requires arithmetic value type");

  public:
    explicit constexpr Matrix3(const std::array<T, 9>& newMatrix) : matrix_(newMatrix) {}
    explicit constexpr Matrix3() noexcept : matrix_(){}

    [[nodiscard]] static constexpr Matrix3 Identity() noexcept {
      return Matrix3(std::array<T,9>{
          T(1), T(0), T(0),
          T(0), T(1), T(0),
          T(0), T(0), T(1)
      });
    }

    [[nodiscard]] T Det() const noexcept {
      const T toAdd =
          matrix_[0] * matrix_[4] * matrix_[8] +
          matrix_[1] * matrix_[5] * matrix_[6] +
          matrix_[2] * matrix_[3] * matrix_[7];

      const T toSub =
          matrix_[6] * matrix_[4] * matrix_[2] +
          matrix_[7] * matrix_[5] * matrix_[0] +
          matrix_[8] * matrix_[3] * matrix_[1];

      return toAdd - toSub;
    }

    [[nodiscard]] Matrix3 Transpose() const noexcept {
      Matrix3 result;
      for (std::size_t i = 0; i < 3; ++i) {
        for (std::size_t j = 0; j < 3; ++j) {
          result(i, j) = (*this)(j, i);
        }
      }
      return result;
    }

    [[nodiscard]] Matrix3 Inverse() const {
      const T det = Det();
      if (det == static_cast<T>(0))
        throw std::runtime_error("Matrix not invertible (determinant = 0)");

      Matrix3 cof;
      for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
          std::array<T, 4> minor;
          size_t idx = 0;

          for (size_t r = 0; r < 3; ++r) {
            if (r == i) continue;
            for (size_t c = 0; c < 3; ++c) {
              if (c == j) continue;
              minor[idx++] = (*this)(r, c);
            }
          }

          T minorDet = minor[0] * minor[3] - minor[1] * minor[2]; // Determinant 2x2

          T sign = (i+j) % 2 == 0 ? static_cast<T>(1) : static_cast<T>(-1);

          cof(i, j) = sign * minorDet;
        }
      }

      Matrix3 adj = cof.Transpose();

      for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 3; ++j)
          adj(i, j) /= det;

      return adj;
    }

    template<typename I>
    T& operator()(I i, I j) {
      static_assert(std::is_integral_v<I>, "Indices must be integral");
      return matrix_[static_cast<size_t>(i) * 3 + static_cast<size_t>(j)];
    }

    template<typename I>
    const T& operator()(I i, I j) const {
      static_assert(std::is_integral_v<I>, "Indices must be integral");
      return matrix_[static_cast<size_t>(i) * 3 + static_cast<size_t>(j)];
    }

    T operator*(const Matrix3& other) {
      const Matrix3 result;
      for (size_t i = 0; i < 3; ++i) {
        for (size_t j = 0; j < 3; ++j) {
          for (size_t k = 0; k < 3; ++k)
            result(i, j) += matrix_(i, k) * other(k, j);
        }
      }
      return result;
    }

  private:
    std::array<T, 9> matrix_;
  };
}

#endif //MATRIX3_H