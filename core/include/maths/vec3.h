#ifndef CORE_MATHS_VEC3_H
#define CORE_MATHS_VEC3_H

#include <cmath>
#include <concepts>

namespace core::maths {
template <typename Vector3T, typename T>
concept IsVector3 = requires(Vector3T v) {
  { v.x } -> std::same_as<T&>;
  { v.y } -> std::same_as<T&>;
  { v.z } -> std::same_as<T&>;
};

template <typename Vector3T, typename T>
concept IsConvertibleVector3 = requires(Vector3T v) {
  { v.x } -> std::convertible_to<T&>;
  { v.y } -> std::convertible_to<T&>;
  { v.z } -> std::convertible_to<T&>;
};

template <typename T>
  requires std::is_arithmetic_v<T>
struct Vec3 {
  T x{}, y{}, z{};
  constexpr Vec3() = default;

  constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z){}

  template <typename VectorT>
    requires IsVector3<VectorT, T>
  constexpr explicit Vec3(VectorT v) : x(v.x), y(v.y), z(v.z){}

  template <typename VectorT>
    requires (IsConvertibleVector3<VectorT, T> && !IsVector3<VectorT, T>)
             constexpr explicit Vec3(VectorT v)
      : x(static_cast<T>(v.x)),
  y(static_cast<T>(v.y)), z(static_cast<T>(v.z))
  {
  }

  [[nodiscard]] constexpr Vec3 operator+(const Vec3& other) const
  {
    return Vec3(x + other.x, y + other.y, z + other.z);
  }

  [[nodiscard]] constexpr Vec3 operator-(const Vec3& other) const
  {
    return Vec3(x - other.x, y - other.y, z - other.z);
  }

  [[nodiscard]] constexpr Vec3 operator*(const Vec3& other) const
  {
    return {x * other.x, y * other.y, z * other.z};
  }

  [[nodiscard]] constexpr Vec3 operator*(const T t) const
  {
    return {x * t, y * t, z * t};
  }

  [[nodiscard]] friend constexpr Vec3 operator*(const T t, const Vec3& other)
  {
    return other * t;
  }

  [[nodiscard]] constexpr T Dot(const Vec3& other) const
  {
    return x * other.x + y * other.y + z * other.z;
  }

  [[nodiscard]] static constexpr T Dot(const Vec3& v1, const Vec3& v2)
  {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
  }

  [[nodiscard]] constexpr Vec3 operator/(const Vec3& other) const
  {
    return {x / other.x, y / other.y, z / other.z};
  }

  [[nodiscard]] constexpr Vec3 operator/(const T t) const
  {
    return {x / t, y / t, z / t};
  }

};
using Vec3F = Vec3<float>;
using Vec3I = Vec3<int32_t>;
using Vec3U = Vec3<uint32_t>;
}  // namespace core::maths

#endif  // CORE_MATHS_VEC3_H
