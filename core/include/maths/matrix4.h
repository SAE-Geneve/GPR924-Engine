//
// Created by hugze on 06.10.2025.
//

#ifndef MATRIX4_H
#define MATRIX4_H

#include <array>

namespace core::maths {
template <typename Matrix4T>
requires std::is_arithmetic_v<Matrix4T>
class Matrix4 {
public:
  explicit Matrix4(std::array<std::array<Matrix4T, 4>, 4> newMatrix) {
    matrix = newMatrix;
  }

  Matrix4T Det() {
    Matrix4T toAdd = 0;
    Matrix4T toSub = 0;

    toAdd += matrix[0][0] * matrix[1][1] * matrix[2][2];
    toAdd += matrix[0][1] * matrix[1][2] * matrix[2][0];
    toAdd += matrix[0][2] * matrix[1][0] * matrix[2][1];
    toAdd += matrix[0][3] * matrix[1][3] * matrix[2][3];

    toSub += matrix[2][0] * matrix[1][1] * matrix[0][2];
    toSub += matrix[2][1] * matrix[1][2] * matrix[0][0];
    toSub += matrix[2][2] * matrix[1][0] * matrix[0][1];
    toSub += matrix[2][3] * matrix[1][3] * matrix[0][3];

    return toAdd - toSub;
  }

private:
  std::array<std::array<Matrix4T, 4>, 4> matrix;
};
}


#endif //MATRIX4_H
