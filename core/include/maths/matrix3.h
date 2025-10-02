#ifndef MATRIX3_H
#define MATRIX3_H
#include <array>

namespace core::maths {
  template <typename T>
  requires std::is_arithmetic_v<T>
  class Matrix3 {
  public:
    explicit Matrix3(auto newMatrix) {
      matrix = newMatrix;
    }

    T Det() {
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

  private:
    std::array<std::array<T, 3>, 3> matrix;
  };
}

#endif //MATRIX3_H
