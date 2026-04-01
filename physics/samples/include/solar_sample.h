//
// Created by hugze on 21.10.2025.
//

#ifndef PHYSIC_SOLAR_SYSTEM_H
#define PHYSIC_SOLAR_SYSTEM_H

#include "body.h"
#include "engine/renderer.h"
#include "maths/vec2.h"
#include "sample.h"
#include "world.h"

namespace solar {
struct PhysicPlanet: common::DrawInterface {

private:
  std::string name_ = "no name";
  core::Vec2F center_of_mass_ = {0.f, 0.f};

  float radius_ = 10;
  SDL_FColor color_{0.f, 0.f, 0.f, 0.f};

  float orbit_radius_ = 0.f;
  float orbit_angle_ = 0.f;

  core::Index<physics::Body> body_index_;

public:
  explicit PhysicPlanet(const std::string_view& name, core::Vec2F center,
                        core::Vec2F offset, float radius,
                        core::Index<physics::Body> idx,
                        SDL_FColor color): body_index_(idx){
    // Initialisation
    center_of_mass_ = center;  // centre de rotation
    body().position = center_of_mass_ + offset;

    // Initial orbit
    const core::Vec2F pos = center_of_mass_ - body().position;
    orbit_radius_ = pos.magnitude();
    orbit_angle_ = atan2(offset.y, offset.x);

    name_ = name;
    radius_ = radius;
    color_ = color;
  }

  void Draw() override{
    if (const auto* renderer = common::GetRenderer(); !renderer) return;
    common::DrawCircle(body().position.x, body().position.y, radius_, color_);
  }

  [[nodiscard]] float orbitAngle() const { return orbit_angle_; }
  [[nodiscard]] float orbitRadius() const { return orbit_radius_; }
  [[nodiscard]] core::Index<physics::Body> bodyIndex() const { return body_index_; }
  [[nodiscard]] physics::Body& body() const { return physics::body_at(body_index_); }
  [[nodiscard]] std::string name() const { return name_; }
};


class SolarSample final : public Sample {
 public:
  explicit SolarSample(core::Vec2<float> ScreenSize = {0.f, 0.f})
      : center_of_system_(ScreenSize / 2.f) {};

  void Begin() override;
  void End() override;
  void FixedUpdate() override;
  void Update([[maybe_unused]] float dt) override;
  void OnGui() override;



  void ApplyGravitationForces() const;

  static float constexpr G = 500.f;
  static float constexpr SunMass = 500000.0f;

 private:
  std::vector<PhysicPlanet> planets_;
  core::Vec2F center_of_system_;

  void CreatePlanet(const std::string_view& name, core::Vec2F center,
                    core::Vec2F offset, float radius, float mass,
                    SDL_FColor color);

 public:
  void Draw() override;
};
}  // namespace solar

#endif  // PHYSIC_SOLAR_SYSTEM_H
