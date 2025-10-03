#ifndef CORE_MATHS_ANGLE_H
#define CORE_MATHS_ANGLE_H

#include <numbers>

namespace core::maths {
struct Degree;

struct Radian {
  constexpr explicit Radian(float value) : value(value) {}

  Radian(const Degree& degree);

  explicit constexpr operator float() const { return value; }

  [[nodiscard]] float Value() const { return value; };

 private:
  float value;
};

struct Degree {
  constexpr explicit Degree(float value) : value(value) {}

  Degree(const Radian& radian)
      : value(radian.Value() * 180.0f / std::numbers::pi_v<float>) {}

  explicit constexpr operator float() const { return value; }

  [[nodiscard]] float Value() const { return value; }

 private:
  float value;
};

inline Radian::Radian(const Degree& degree)
    : value(degree.Value() * std::numbers::pi_v<float> / 180.0f) {}
}  // namespace core::maths

#endif  // CORE_MATHS_ANGLE_H