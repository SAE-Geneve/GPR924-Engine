//
// Created by hugze on 21.10.2025.
//

#include <imgui.h>

#include "solar_sample.h"
#include "engine/window.h"
#include "world.h"

namespace solar {
void SolarSample::CreatePlanet(const std::string_view& name,
                                     core::Vec2F center, core::Vec2F offset,
                                     float radius, float mass,
                                     SDL_FColor color) {
  core::Index<physics::Body> idx = physics::AddBody(mass);
  PhysicPlanet planet(name, center, offset, radius, idx, color);
  planets_.push_back(planet);

  // Initial velocity
  float velocity = std::sqrt(G * offset.x / SunMass);
  planets_.back().body().Velocity({0, velocity});
}
void SolarSample::Draw() {
  if (const auto* renderer = common::GetRenderer(); !renderer) return;
  for (auto& p : planets_) {
    p.Draw();
  }
}

void SolarSample::Begin() {
  // Create Sun
  core::Index<physics::Body> idx = physics::AddBody(SunMass);
  PhysicPlanet sun("Sun", center_of_system_, {0, 0}, 50,
                   idx, {1.f, 1.f, 0.f, 1.f});
  planets_.push_back(sun);

  // Create planets
  CreatePlanet("Mercure", {80, 0}, center_of_system_, 3, 5,
               {1.f, 0.9f, 0.5f, 1.f});
  CreatePlanet("Venus", {100, 0}, center_of_system_, 6, 15,
               {1.f, 0.9f, 0.5f, 1.f});
  CreatePlanet("Earth", {150, 0}, center_of_system_, 10, 10,
               {0.f, 0.6f, 1.f, 1.f});
  CreatePlanet("Mars", {250, 0}, center_of_system_, 8, 5,
               {1.f, 0.3f, 0.2f, 1.f});
  CreatePlanet("Jupiter", {400, 0}, center_of_system_, 20, 318,
               {1.f, 0.7f, 0.5f, 1.f});
  CreatePlanet("Saturn", {600, 0}, center_of_system_, 18, 95,
               {1.f, 1.f, 0.6f, 1.f});
  CreatePlanet("Uranus", {800, 0}, center_of_system_, 14, 14,
               {0.5f, 1.f, 1.f, 1.f});
  CreatePlanet("Neptune", {900, 0}, center_of_system_, 14, 17,
               {0.2f, 0.4f, 1.f, 1.f});

  common::DrawObserverSubject::AddObserver(this);
}

void SolarSample::ApplyGravitationForces() const {
  for (size_t i = 0; i < planets_.size(); ++i) {
    for (size_t j = i + 1; j < planets_.size(); ++j) {
      physics::Body& body1 = planets_[i].body();
      physics::Body& body2 = planets_[j].body();

      if (body1.IsInvalid() || body2.IsInvalid()) continue;

      core::Vec2F direction = body2.position - body1.position;
      float distanceSq = direction.magnitude_sqr();

      if (distanceSq < 25.0f) continue;

      float distance = std::sqrt(distanceSq);
      direction = direction / distance;  // Normaliser

      float forceMagnitude = G * (body1.mass() * body2.mass()) / distanceSq;

      if (forceMagnitude > 10000.0f) {
        forceMagnitude = 10000.0f;
      }
      core::Vec2F force = direction * forceMagnitude;
      // Appliquer les forces (3ème loi de Newton)
      body1.AddForce(force);
      body2.AddForce(-force);
    }
  }
}

void SolarSample::FixedUpdate() {
  ApplyGravitationForces();
  physics::Tick(common::GetFixedDT());
}

void SolarSample::Update(float dt) {}

void SolarSample::End() {
  for (auto& p : planets_) {
    physics::RemoveBody(p.bodyIndex());
  }
  planets_.clear();
  physics::SetContactListener(nullptr);
  common::DrawObserverSubject::RemoveObserver(this);
}

void SolarSample::OnGui() {
  ImGui::Begin("Info Solar System");
  for (PhysicPlanet& planet : planets_) {
    ImGui::Text(" %d Pos %s : %f %f", planet.bodyIndex().index(), planet.name().c_str(),
                planet.body().position.x, planet.body().position.y);
  }
  ImGui::End();
}
}  // namespace solar