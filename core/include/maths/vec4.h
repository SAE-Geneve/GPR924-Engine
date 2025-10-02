#ifndef VEC4_H
#define VEC4_H

#include <cmath>
#include <concepts>

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
    requires IsConvertibleVector4<VectorT, T> && !IsVector4<VectorT, T>
             constexpr explicit Vec4(VectorT v)
      : x(static_cast<T>(v.x)),
  y(static_cast<T>(v.y)), z(static_cast<T>(v.z)), w(static_cast<T>(v.w))
  {
  }

  [[nodiscard]] constexpr Vec4 operator+(const Vec4& other) const
  {
    return Vec4(x + other.x, y + other.y, z + other.z, w + other.w);
  }

  [[nodiscard]] constexpr Vec4 operator-(const Vec4& other) const
  {
    return Vec4(x - other.x, y - other.y, z - other.z, w - other.w);
  }

  [[nodiscard]] constexpr Vec4 operator*(const Vec4& other) const
  {
    return {x * other.x, y * other.y, z * other.z, w * other.w};
  }

  [[nodiscard]] constexpr Vec4 operator*(const T t) const
  {
    return {x * t, y * t, z * t, w * t};
  }

  [[nodiscard]] friend constexpr Vec4 operator*(const T t, const Vec4& other)
  {
    return other * t;
  }

  [[nodiscard]] constexpr T Dot(const Vec4& other) const
  {
    return x * other.x + y * other.y + z * other.z + w * other.w;
  }

  [[nodiscard]] static constexpr T Dot(const Vec4& v1, const Vec4& v2)
  {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
  }

  [[nodiscard]] constexpr Vec4 operator/(const Vec4& other) const
  {
    return {x / other.x, y / other.y, z / other.z, w / other.w};
  }

  [[nodiscard]] constexpr Vec4 operator/(const T t) const
  {
    return {x / t, y / t, z / t, w / t};
  }

};
using Vec4F = Vec4<float>;
using Vec4I = Vec4<int32_t>;
using Vec4U = Vec4<uint32_t>;
}  // namespace core::maths

#endif  // VEC4_H
