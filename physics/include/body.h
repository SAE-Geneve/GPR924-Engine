//
// Created by hugze on 20.10.2025.
//

#ifndef BODY_H
#define BODY_H

#include "maths/vec2.h"

namespace physics {

class Body {
 public:
  explicit Body(float mass);
  core::Vec2F position = {0, 0};
  void Velocity(core::Vec2F vel);
  void AddForce(core::Vec2F force);
  void Tick(float dt);

  [[nodiscard]] bool IsInvalid() const { return mass_ <= 0.f; }
  void SetAsInvalid() { mass_ = -1; }
  [[nodiscard]] static Body GenerateInvalidValue() { return Body(-1); }

  [[nodiscard]] float mass() const { return mass_; }
  [[nodiscard]] core::Vec2F velocity() const { return velocity_; }

 private:
  float mass_ = 0.1f;
  core::Vec2F velocity_ = {0, 0};
  core::Vec2F accumulated_force = {0, 0};
};
}  // namespace physics

#endif  // BODY_H
