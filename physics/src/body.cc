//
// Created by hugze on 20.10.2025.
//

#include "body.h"

namespace physics {
Body::Body(const float mass) :  mass_(mass) {}

void Body::Velocity(core::Vec2F vel) {
  velocity_ = vel;
}

void Body::AddForce(core::Vec2F force) {
  accumulated_force += force;
}

void Body::Tick(float dt) {
  core::Vec2F acceleration = accumulated_force / mass_;

  velocity_ += acceleration * dt;

  position += velocity_ * dt;

  accumulated_force = {0, 0};
}

}  // namespace physics