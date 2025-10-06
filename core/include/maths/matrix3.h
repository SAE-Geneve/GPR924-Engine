#ifndef MATRIX3_H
#define MATRIX3_H
#include <array>

namespace core::maths {
  template <typename T>
  class Matrix3 {
    static_assert(std::is_arithmetic_v<T>, "Matrix3 requires arithmetic value type");

  public:
    explicit Matrix3(std::array<std::array<T, 3>, 3> newMatrix) {
      matrix = newMatrix;
    }

    explicit Matrix3() noexcept {
      matrix = {
        {0, 0, 0},
        {0, 0, 0},
        {0, 0, 0}
      };
    }

    [[nodiscard]] static Matrix3 Identity() noexcept {
      return Matrix3(std::array<std::array<T, 3>, 3>{
          std::array<T,3>{1,0,0},
          std::array<T,3>{0,1,0},
          std::array<T,3>{0,0,1}
      });
    }

    [[nodiscard]] T Det() {
      T toAdd = 0;
      T toSub = 0;

      toAdd += matrix[0][0] * matrix[1][1] * matrix[2][2];
      toAdd += matrix[0][1] * matrix[1][2] * matrix[2][0];
      toAdd += matrix[0][2] * matrix[1][0] * matrix[2][1];

      toSub += matrix[2][0] * matrix[1][1] * matrix[0][2];
      toSub += matrix[2][1] * matrix[1][2] * matrix[0][0];
      toSub += matrix[2][2] * matrix[1][0] * matrix[0][1];

      return toAdd - toSub;
    }

    T& operator()(size_t i, size_t j) { return matrix[i][j]; }
    const T& operator()(size_t i, size_t j) const { return matrix[i][j]; }

  private:
    std::array<std::array<T, 3>, 3> matrix;
  };
}

#endif //MATRIX3_H
