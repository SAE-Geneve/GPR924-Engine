#ifndef CORE_MATHS_VEC2_H
#define CORE_MATHS_VEC2_H

#include <cmath>
#include <concepts>

#include "angle.h"

namespace core::maths {
// Templates allow to write code for any type of data
// Typename is a parameter for templates that takes in any type of data
template <typename Vector2T, typename T>
// Concept is a named set of requirements that a type must satisfy to be used in
// a given context This concept checks if the type is a vector 2 and if both
// points of the vectors match to the type T
concept IsVector2 = requires(Vector2T v) {
  { v.x } -> std::same_as<T&>;
  { v.y } -> std::same_as<T&>;
};

template <typename Vector2T, typename T>
// This concept checks if both points of the vector 2 can be converted to the
// type T
concept IsConvertibleVector2 = requires(Vector2T v) {
  { v.x } -> std::convertible_to<T&>;
  { v.y } -> std::convertible_to<T&>;
};

template <typename T>
  requires std::is_arithmetic_v<T>
// Sets up the struct of Vector 2 by giving it two points
struct Vec2 {
  T x{}, y{};
  // Compiler gives the default body in case none is given
  constexpr Vec2() = default;

  // Constructor for the vec 2
  constexpr Vec2(T x, T y) : x(x), y(y) {}

  template <typename VectorT>
    requires IsVector2<VectorT, T>
  constexpr explicit Vec2(VectorT v) : x(v.x), y(v.y) {}

  // Converts the vec2 to a given new data type
  template <typename VectorT>
  // Verify beforehand if it is possible to convert by using previous concept
    requires(IsConvertibleVector2<VectorT, T> && !IsVector2<VectorT, T>)
  constexpr explicit Vec2(VectorT v)
      : x(static_cast<T>(v.x)), y(static_cast<T>(v.y)) {}

  // nodiscard makes it so the result of the function is not ignored (Safety
  // purposes) constexpr makes it so that the result can be determined during
  // compile time operator overloads the normal use of the operator in the case
  // it is used for a Vec2 struct
  [[nodiscard]] constexpr Vec2 operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
  }

  [[nodiscard]] constexpr Vec2 operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
  }

  [[nodiscard]] constexpr Vec2 operator*(const Vec2& other) const {
    return {x * other.x, y * other.y};
  }
  [[nodiscard]] constexpr Vec2 operator*(const T t) const {
    return {x * t, y * t};
  }
  [[nodiscard]] friend constexpr Vec2 operator*(const T t, const Vec2& other) {
    return other * t;
  }

  [[nodiscard]] constexpr T Dot(const Vec2& other) const {
    return x * other.x + y * other.y;
  }
  [[nodiscard]] static constexpr T Dot(const Vec2& v1, const Vec2& v2) {
    return v1.x * v2.x + v1.y * v2.y;
  }
  //Delete this
  [[nodiscard]] constexpr Vec2 Cross(const Vec2& other) const {
    return Vec2{y * other.x - x * other.y, x * other.y - y * other.x};
  }
  //Delete this
  [[nodiscard]] static constexpr Vec2 Cross(const Vec2& v1, const Vec2& v2) {
    return Vec2{v1.y * v2.x - v1.x * v2.y, v1.x * v2.y - v1.y * v2.x};
  }

  [[nodiscard]] constexpr Vec2 operator/(const Vec2& other) const {
    return {x / other.x, y / other.y};
  }
  [[nodiscard]] constexpr Vec2 operator/(const T t) const {
    return {x / t, y / t};
  }

  [[nodiscard]] constexpr T magnitude_sqr() const
    requires std::is_floating_point_v<T>
  {
    return x * x + y * y;
  }
  [[nodiscard]] T magnitude() const
    requires std::is_floating_point_v<T>
  {
    return std::sqrt(magnitude_sqr());
  }

  [[nodiscard]] Vec2 Normalize() const
    requires std::is_floating_point_v<T>
  {
    const T man = magnitude();
    if (man == 0.0f) {
      return Vec2(0, 0);
    }
    return *this / man;
  }
  //Delete this, make a simple Rotate function with a rotation matrix
  [[nodiscard]] Vec2 RotateX(const Radian angle) const
    requires std::floating_point<T>
  {
    const T c = std::cos(static_cast<float>(angle));
    const T new_y = y * c;
    return Vec2(x, new_y);
  }

  [[nodiscard]] Vec2 RotateY(const Radian angle) const
    requires std::floating_point<T>
  {
    const T c = std::cos(static_cast<float>(angle));
    const T new_x = x * c;
    return Vec2(new_x, y);
  }

  [[nodiscard]] static constexpr Vec2 Lerp(const Vec2 v0, const Vec2 v1,
                                           const float t)
    requires std::is_floating_point_v<T>
  {
    return Vec2{v0.x * (1 - t) + v1.x * t, v0.y * (1 - t) + v1.y * t};
  }

  [[nodiscard]] static Vec2 Slerp(const Vec2 v0, const Vec2 v1, const float t)
    requires std::is_floating_point_v<T>
  {
    T dot = v0.Dot(v1);

    if (dot > 1.0f) dot = 1.0f;
    if (dot < -1.0f) dot = -1.0f;

    const float theta = std::acos(dot);
    const float sinTheta = std::sin(theta);

    if (sinTheta < 1e-6f) {
      return Vec2{v0.x * (1 - t) + v1.x * t, v0.y * (1 - t) + v1.y * t};
    }

    float w0 = std::sin((1 - t) * theta) / sinTheta;
    float w1 = std::sin(t * theta) / sinTheta;

    return Vec2{v0.x * w0 + v1.x * w1, v0.y * w0 + v1.y * w1};
  }

  [[nodiscard]] static constexpr Vec2 Projection(const Vec2 v,
                                                 const Vec2 onto) {
    T dotVV = onto.Dot(onto);
    if (dotVV == 0) return Vec2{0, 0};
    T factor = v.Dot(onto) / dotVV;
    return Vec2{onto.x * factor, onto.y * factor};
  }

  [[nodiscard]] static constexpr Vec2 Reflection(const Vec2 v,
                                                 const Vec2 normal) {
    T factor = 2 * v.Dot(normal);
    return Vec2{v.x - normal.x * factor, v.y - normal.y * factor};
  }
};

using Vec2F = Vec2<float>;
using Vec2I = Vec2<int32_t>;
using Vec2U = Vec2<uint32_t>;

}  // namespace core::maths
#endif  // CORE_MATHS_VEC2_H
