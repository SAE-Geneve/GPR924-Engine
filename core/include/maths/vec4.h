#ifndef CORE_MATHS_VEC4_H
#define CORE_MATHS_VEC4_H

#include <cmath>
#include <concepts>

#include "angle.h"

namespace core::maths {

template <typename Vector4T, typename T>
concept IsVector4 = requires(Vector4T v) {
  { v.x } -> std::same_as<T&>;
  { v.y } -> std::same_as<T&>;
  { v.z } -> std::same_as<T&>;
  { v.w } -> std::same_as<T&>;
};

template <typename Vector4T, typename T>
concept IsConvertibleVector4 = requires(Vector4T v) {
  { v.x } -> std::convertible_to<T&>;
  { v.y } -> std::convertible_to<T&>;
  { v.z } -> std::convertible_to<T&>;
  { v.w } -> std::convertible_to<T&>;
};

template <typename T>
  requires std::is_arithmetic_v<T>
struct Vec4 {
  T x{}, y{}, z{}, w{};
  constexpr Vec4() = default;

  constexpr Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

  template <typename VectorT>
    requires IsVector4<VectorT, T>
  constexpr explicit Vec4(VectorT v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

  template <typename VectorT>
    requires(IsConvertibleVector4<VectorT, T> && !IsVector4<VectorT, T>)
  constexpr explicit Vec4(VectorT v)
      : x(static_cast<T>(v.x)),
        y(static_cast<T>(v.y)),
        z(static_cast<T>(v.z)),
        w(static_cast<T>(v.w)) {}

  [[nodiscard]] constexpr Vec4 operator+(const Vec4& other) const {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  [[nodiscard]] constexpr Vec4 operator-(const Vec4& other) const {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  [[nodiscard]] constexpr Vec4 operator*(const Vec4& other) const {
    return {x * other.x, y * other.y, z * other.z, w * other.w};
  }

  [[nodiscard]] constexpr Vec4 operator*(const T t) const {
    return {x * t, y * t, z * t, w * t};
  }

  [[nodiscard]] friend constexpr Vec4 operator*(const T t, const Vec4& other) {
    return other * t;
  }

  [[nodiscard]] constexpr T Dot(const Vec4& other) const {
    return x * other.x + y * other.y + z * other.z + w * other.w;
  }

  [[nodiscard]] static constexpr T Dot(const Vec4& v1, const Vec4& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
  }

  [[nodiscard]] constexpr Vec4 operator/(const Vec4& other) const {
    return {x / other.x, y / other.y, z / other.z, w / other.w};
  }

  [[nodiscard]] constexpr Vec4 operator/(const T t) const {
    return {x / t, y / t, z / t, w / t};
  }

  [[nodiscard]] T MagnitudeSqr() const
    requires std::is_floating_point_v<T>
  {
    return x * x + y * y + z * z + w * w;
  }

  [[nodiscard]] T Magnitude() const
    requires std::is_floating_point_v<T>
  {
    return std::sqrt(MagnitudeSqr());
  }

  [[nodiscard]] Vec4 Normalize() const
    requires std::is_floating_point_v<T>
  {
    const T man = Magnitude();
    if (man == 0.0f) {
      return Vec4(0, 0, 0, 0);
    }
    return *this / man;
  }

  [[nodiscard]] static constexpr Vec4 Lerp(const Vec4 v0, const Vec4 v1,
                                           const float t)
    requires std::is_floating_point_v<T>
  {
    return Vec4{v0.x * (1 - t) + v1.x * t, v0.y * (1 - t) + v1.y * t,
                v0.z * (1 - t) + v1.z * t, v0.w * (1 - t) + v1.w * t};
  }

  [[nodiscard]] static constexpr Vec4 Projection(const Vec4 v,
                                                 const Vec4 onto) {
    T dotVV = onto.Dot(onto);
    if (dotVV == 0) return Vec4{0, 0, 0, 0};
    T factor = v.Dot(onto) / dotVV;
    return Vec4{
        onto.x * factor,
        onto.y * factor,
        onto.z * factor,
        onto.w * factor,
    };
  }

  [[nodiscard]] static constexpr Vec4 Reflection(const Vec4 v,
                                                 const Vec4 normal) {
    T factor = 2 * v.Dot(normal);
    return Vec4{
        v.x - normal.x * factor,
        v.y - normal.y * factor,
        v.z - normal.z * factor,
        v.w - normal.w * factor,
    };
  }
};
using Vec4F = Vec4<float>;
using Vec4I = Vec4<int32_t>;
using Vec4U = Vec4<uint32_t>;

}  // namespace core::maths

#endif  // CORE_MATHS_VEC4_H
