//
// Created by hugze on 14.11.2025.
//

#include "bounce_sample.h"

#include "engine/window.h"
#include "utils/colors.h"

namespace colliders {
    void BounceSample::Begin(){
        rendered_objects_.reserve(10);

        //Create ground
        core::Index<physics::Body> ground_body_idx = physics::AddBody(10000000000.f);
        physics::AABB ground_aabb(core::Vec2F{screen_size.x, screen_size.y});
        core::Index<physics::Collider> ground_collider_idx = physics::AddColliderToBody(
            ground_body_idx, {0.f, 0.f}, 1, ground_aabb, physics::ShapeType::AABB, false);

        physics::body_at(ground_body_idx).position = core::Vec2F{screen_size.x / 2, screen_size.y * 1.4f};
        physics::body_at(ground_body_idx).Velocity({0, 0});

        utils::RenderedObject r_ground{ground_collider_idx, kWhite};
        rendered_objects_.emplace_back(r_ground);

        //Create bouncy AABB
        core::Index<physics::Body> aabb_body_idx = physics::AddBody(10);
        physics::AABB aabb(core::Vec2F{100, 100});
        core::Index<physics::Collider> aabb_collider_idx = physics::AddColliderToBody(
            aabb_body_idx, {0.f, 0.f}, 1, aabb, physics::ShapeType::AABB, false);

        physics::body_at(aabb_body_idx).position = core::Vec2F{screen_size.x / 2 - 100, screen_size.y / 2};
        physics::body_at(aabb_body_idx).Velocity({0, 0});

        utils::RenderedObject r_aabb{aabb_collider_idx, kGreen};
        rendered_objects_.emplace_back(r_aabb);

        //Create not bouncy circle
        core::Index<physics::Body> circle_body_idx = physics::AddBody(10);
        physics::Circle circle(50);
        core::Index<physics::Collider> circle_collider_idx = physics::AddColliderToBody(
            circle_body_idx, {0.f, 0.f}, 0, circle, physics::ShapeType::Circle, false);

        physics::body_at(circle_body_idx).position = core::Vec2F{screen_size.x / 2 + 100, screen_size.y / 2};
        physics::body_at(circle_body_idx).Velocity({0, 0});

        utils::RenderedObject r_circle{circle_collider_idx, kRed};
        rendered_objects_.emplace_back(r_circle);

        physics::set_contact_listener(this);
        common::DrawObserverSubject::AddObserver(this);
    }

    void BounceSample::End(){
        physics::set_contact_listener(nullptr);
        for (auto &r: rendered_objects_) {
            if (r.collider().IsInvalid()) continue;
            physics::RemoveBody(r.body_idx());
            physics::RemoveCollider(r.collider_idx);
        }
        rendered_objects_.clear();
        common::DrawObserverSubject::RemoveObserver(this);
    }

    void BounceSample::FixedUpdate(){
        ApplyGravitationForceTowardsGround();
        physics::Tick(common::GetFixedDT());
    }

    void BounceSample::Update([[maybe_unused]] float dt){
    }

    void BounceSample::OnGui(){
        // ImGui::Begin("Bounce Sample");
        // ImGui::End();
    }

    void BounceSample::Draw(){
        if (const auto *renderer = common::GetRenderer(); !renderer) return;
        for (auto &r: rendered_objects_) {
            r.Draw();
        }
    }

    void BounceSample::ApplyGravitationForceTowardsGround() const {
        physics::Body& ground = rendered_objects_[0].body();
        for (size_t i = 1; i < rendered_objects_.size(); ++i) {
                physics::Body& body = rendered_objects_[i].body();

                if (body.IsInvalid() || ground.IsInvalid()) continue;

                core::Vec2F direction = {0, ground.position.y - body.position.y};
                float distanceSq = direction.magnitude_sqr();

                if (distanceSq < 25.0f) continue;

                float distance = std::sqrt(distanceSq);
                direction = direction / distance;  // Normaliser

                float forceMagnitude = G * (body.mass() * ground.mass()) / distanceSq;

                if (forceMagnitude > 10000.0f) {
                    forceMagnitude = 10000.0f;
                }
                core::Vec2F force = direction * forceMagnitude;
                // Appliquer les forces (3ème loi de Newton)
                body.AddForce(force);
                ground.AddForce(-force);
        }
    }

    void BounceSample::OnTriggerEnter(physics::CollidersPair cols) {
    }

    void BounceSample::OnTriggerExit(physics::CollidersPair cols){
    }

    void BounceSample::OnColliderEnter(physics::CollidersPair cols){
    }

    void BounceSample::OnColliderExit(physics::CollidersPair cols){
    }
}
