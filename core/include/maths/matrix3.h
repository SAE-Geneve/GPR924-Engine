#ifndef MATRIX3_H
#define MATRIX3_H
#include <array>

namespace core::maths {
  template <typename T>
  class Matrix3 {
    static_assert(std::is_arithmetic_v<T>, "Matrix3 requires arithmetic value type");

  public:
    explicit constexpr Matrix3(const std::array<T, 9>& newMatrix) : matrix(newMatrix) {}

    explicit constexpr Matrix3() noexcept
        : matrix({ T(0), T(0), T(0),
                   T(0), T(0), T(0),
                   T(0), T(0), T(0) }) {}

    [[nodiscard]] static constexpr Matrix3 Identity() noexcept {
      return Matrix3(std::array<T,9>{
          T(1), T(0), T(0),
          T(0), T(1), T(0),
          T(0), T(0), T(1)
      });
    }

    [[nodiscard]] T Det() const noexcept {
      const T toAdd =
          matrix[0] * matrix[4] * matrix[8] +
          matrix[1] * matrix[5] * matrix[6] +
          matrix[2] * matrix[3] * matrix[7];

      const T toSub =
          matrix[6] * matrix[4] * matrix[2] +
          matrix[7] * matrix[5] * matrix[0] +
          matrix[8] * matrix[3] * matrix[1];

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

    template<typename I>
    T& operator()(I i, I j) {
      static_assert(std::is_integral_v<I>, "Indices must be integral");
      return matrix[static_cast<std::size_t>(i) * 3 + static_cast<std::size_t>(j)];
    }

    template<typename I>
    const T& operator()(I i, I j) const {
      static_assert(std::is_integral_v<I>, "Indices must be integral");
      return matrix[static_cast<std::size_t>(i) * 3 + static_cast<std::size_t>(j)];
    }

  private:
    std::array<T, 9> matrix;
  };
}

#endif //MATRIX3_H