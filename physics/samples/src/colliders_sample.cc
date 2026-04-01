//
// Created by hugze on 03.11.2025.
//

#include "colliders_sample.h"

#include <imgui.h>

#include <random>

#include "engine/renderer.h"
#include "engine/window.h"
#include "utils/colors.h"
#include "quadtree_debug_draw.h"

namespace colliders {
    static constexpr SDL_FColor kTriggerColor = kRed;
    static constexpr SDL_FColor kCollisionColor = kGreen;
    static constexpr SDL_FColor kNoOverlapColor = kWhite;

    // Create a random number generator (put this as a member variable)
    std::mt19937 rng{std::random_device{}()}; // Mersenne Twister with random seed

    void CollidersSample::Begin() {
        std::uniform_real_distribution vel_dist_x(initial_velocity_range_.first.x,
                                                  initial_velocity_range_.second.x);
        std::uniform_real_distribution vel_dist_y(initial_velocity_range_.first.y,
                                                  initial_velocity_range_.second.y);
        std::uniform_real_distribution radius_dist(initial_radius_range_.first,
                                                   initial_radius_range_.second);
        std::uniform_real_distribution size_dist_x(initial_aabb_size_range_.first.x,
                                                   initial_aabb_size_range_.second.x);
        std::uniform_real_distribution size_dist_y(initial_aabb_size_range_.first.y,
                                                   initial_aabb_size_range_.second.y);
        std::uniform_real_distribution screen_dist_x(10.0f, screen_size.x - 10.0f);
        std::uniform_real_distribution screen_dist_y(10.0f, screen_size.y - 10.0f);

        trigger_mode = false;
        if (rendered_objects_.capacity() < objects_capacity_) {
            rendered_objects_.reserve(objects_capacity_);
        }
        for (int i = 0; i < initial_objects_count_; ++i) {
            if (i % 2 == 0) {
                AddCircle(10.f, {0.f, 0.f}, radius_dist(rng), initial_restitution_, kNoOverlapColor,
                          {screen_dist_x(rng), screen_dist_y(rng)},
                          {vel_dist_x(rng), vel_dist_y(rng)}, trigger_mode);
            } else {
                AddAABB(10.f, {0.f, 0.f}, {size_dist_x(rng), size_dist_y(rng)}, initial_restitution_, kNoOverlapColor,
                        {screen_dist_x(rng), screen_dist_y(rng)},
                        {vel_dist_x(rng), vel_dist_y(rng)}, trigger_mode);
            }
            current_objects_count_++;
        }

        AddWalls();

        physics::set_contact_listener(this);
        common::DrawObserverSubject::AddObserver(this);
    }

    void CollidersSample::End() {
        physics::set_contact_listener(nullptr);
        for (auto &r: rendered_objects_) {
            if (r.collider().IsInvalid()) continue;
            physics::RemoveBody(r.body_idx());
            physics::RemoveCollider(r.collider_idx);
        }
        for (auto &r: wall_objects_) {
            if (r.collider().IsInvalid()) continue;
            physics::RemoveBody(r.body_idx());
            physics::RemoveCollider(r.collider_idx);
        }
        current_objects_count_ = 0;
        rendered_objects_.clear();
        wall_objects_.clear();
        common::DrawObserverSubject::RemoveObserver(this);
    }

    void CollidersSample::AddCircle(float mass, core::Vec2F collider_offset, float radius, float restitution,
                                    SDL_FColor color, core::Vec2F pos,
                                    core::Vec2F vel, bool isTrigger) {
        core::Index<physics::Body> body_idx = physics::AddBody(mass);

        physics::Circle circle(radius);
        core::Index<physics::Collider> collider_idx = physics::AddColliderToBody(
            body_idx, collider_offset, restitution, circle, physics::ShapeType::Circle, isTrigger);

        utils::RenderedObject r{collider_idx, color};

        physics::body_at(body_idx).position = pos;
        physics::body_at(body_idx).Velocity(vel);

        rendered_objects_.emplace_back(r);
    }

    void CollidersSample::AddAABB(float mass, core::Vec2F collider_offset,
                                  core::Vec2F size, float restitution, SDL_FColor color,
                                  core::Vec2F pos, core::Vec2F vel,
                                  bool isTrigger) {
        core::Index<physics::Body> body_idx = physics::AddBody(mass);
        physics::AABB aabb(size);
        core::Index<physics::Collider> collider_idx = physics::AddColliderToBody(
            body_idx, collider_offset, restitution, aabb, physics::ShapeType::AABB, isTrigger);

        utils::RenderedObject r{collider_idx, color};

        physics::body_at(body_idx).position = pos;
        physics::body_at(body_idx).Velocity(vel);

        rendered_objects_.emplace_back(r);
    }

    void CollidersSample::RemoveObject(utils::RenderedObject &r) {
        auto it = std::ranges::find(rendered_objects_, r);
        if (it == rendered_objects_.end()) {
            // Not found — maybe already removed
            return;
        }

        physics::RemoveBody(it->body_idx());
        physics::RemoveCollider(it->collider_idx);

        rendered_objects_.erase(it);
    }

    void CollidersSample::FixedUpdate() {
        SetColor();
        physics::Tick(common::GetFixedDT());
    }

    void CollidersSample::Update([[maybe_unused]] float dt) {
    }

    void CollidersSample::OnGui() {
        ImGui::Begin("Collider Sample");

        ImGui::SeparatorText("Shapes");
        if (ImGui::SliderInt("Objects Count", &current_objects_count_, 0,
                             objects_capacity_)) {
            UpdateObjectsCount();
        }
        ImGui::Text("Shape Type To Add :");
        if (ImGui::RadioButton("Circle",
                               shape_to_add == physics::ShapeType::Circle)) {
            shape_to_add = physics::ShapeType::Circle;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("AABB", shape_to_add == physics::ShapeType::AABB)) {
            shape_to_add = physics::ShapeType::AABB;
        }
        if (ImGui::RadioButton("Random",
                               shape_to_add == physics::ShapeType::Length)) {
            shape_to_add = physics::ShapeType::Length;
        }
        if (ImGui::CollapsingHeader("Shapes Initials Parameters")) {
            if (ImGui::SliderFloat("Min velocity x", &initial_velocity_range_.first.x,
                                   -max_initial_velocity, -1)) {
            }
            if (ImGui::SliderFloat("Max velocity x", &initial_velocity_range_.second.x,
                                   1, max_initial_velocity)) {
            }
            if (ImGui::SliderFloat("Min velocity y", &initial_velocity_range_.first.y,
                                   -max_initial_velocity, -1)) {
            }
            if (ImGui::SliderFloat("Max velocity y", &initial_velocity_range_.second.y,
                                   1, max_initial_velocity)) {
            }

            if (ImGui::SliderFloat("Min radius", &initial_radius_range_.first, 1,
                                   max_initial_radius / 2)) {
            }
            if (ImGui::SliderFloat("Max radius", &initial_radius_range_.second,
                                   max_initial_radius / 2, max_initial_radius)) {
            }

            if (ImGui::SliderFloat("Min AABB size x", &initial_aabb_size_range_.first.x,
                                   1, max_initial_aabb_size / 2)) {
            }
            if (ImGui::SliderFloat("Max AABB size x",
                                   &initial_aabb_size_range_.second.x,
                                   max_initial_aabb_size / 2, max_initial_aabb_size)) {
            }
            if (ImGui::SliderFloat("Min AABB size y", &initial_aabb_size_range_.first.y,
                                   1, max_initial_aabb_size / 2)) {
            }
            if (ImGui::SliderFloat("Max AABB size y",
                                   &initial_aabb_size_range_.second.y,
                                   max_initial_aabb_size / 2, max_initial_aabb_size)) {
            }
            if (ImGui::SliderFloat("Restitution",
                                   &initial_restitution_,
                                   0, max_initial_restitution_)) {
            }
        }

        ImGui::SeparatorText("Trigger Mode");
        if (ImGui::RadioButton("Trigger", trigger_mode == true)) {
            trigger_mode = true;
            for (auto &r: rendered_objects_) {
                r.collider().is_trigger = trigger_mode;
                r.collision_count = 0;
                r.trigger_count = 0;
            }
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Collider", trigger_mode == false)) {
            trigger_mode = false;
            for (auto &r: rendered_objects_) {
                r.collider().is_trigger = trigger_mode;
                r.collision_count = 0;
                r.trigger_count = 0;
            }
        }
        ImGui::SeparatorText("Quadtree");
        if (ImGui::RadioButton("Show", show_quadtree == true)) {
            show_quadtree = true;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Hide", show_quadtree == false)) {
            show_quadtree = false;
        }

        ImGui::End();
    }

    void CollidersSample::UpdateObjectsCount() {
        std::uniform_real_distribution vel_dist_x(initial_velocity_range_.first.x,
                                                  initial_velocity_range_.second.x);
        std::uniform_real_distribution vel_dist_y(initial_velocity_range_.first.y,
                                                  initial_velocity_range_.second.y);
        std::uniform_real_distribution radius_dist(initial_radius_range_.first,
                                                   initial_radius_range_.second);
        std::uniform_real_distribution size_dist_x(initial_aabb_size_range_.first.x,
                                                   initial_aabb_size_range_.second.x);
        std::uniform_real_distribution size_dist_y(initial_aabb_size_range_.first.y,
                                                   initial_aabb_size_range_.second.y);
        std::uniform_int_distribution shape_to_add_rng(1, 2);
        std::uniform_real_distribution screen_dist_x(10.0f, screen_size.x - 10.0f);
        std::uniform_real_distribution screen_dist_y(10.0f, screen_size.y - 10.0f);

        while (current_objects_count_ < rendered_objects_.size()) {
            RemoveObject(rendered_objects_.back());
        }
        while (current_objects_count_ > rendered_objects_.size()) {
            switch (shape_to_add) {
                case physics::ShapeType::AABB:
                    AddAABB(10.f, {0.f, 0.f}, {size_dist_x(rng), size_dist_y(rng)}, initial_restitution_,
                            kNoOverlapColor, {screen_dist_x(rng), screen_dist_y(rng)},
                            {vel_dist_x(rng), vel_dist_y(rng)}, trigger_mode);
                    break;
                case physics::ShapeType::Circle:
                    AddCircle(10.f, {0.f, 0.f}, radius_dist(rng), initial_restitution_, kNoOverlapColor,
                              {screen_dist_x(rng), screen_dist_y(rng)},
                              {vel_dist_x(rng), vel_dist_y(rng)}, trigger_mode);
                    break;
                default:
                    if (shape_to_add_rng(rng) % 2 == 0) {
                        AddCircle(10.f, {0.f, 0.f}, radius_dist(rng), initial_restitution_, kNoOverlapColor,
                                  {screen_dist_x(rng), screen_dist_y(rng)},
                                  {vel_dist_x(rng), vel_dist_y(rng)}, trigger_mode);
                    } else {
                        AddAABB(10.f, {0.f, 0.f}, {size_dist_x(rng), size_dist_y(rng)}, initial_restitution_,
                                kNoOverlapColor, {screen_dist_x(rng), screen_dist_y(rng)},
                                {vel_dist_x(rng), vel_dist_y(rng)}, trigger_mode);
                    }
                    break;
            }
        }
    }

    void CollidersSample::AddWalls() {
        static constexpr float kThickness = 20.f;
        static constexpr SDL_FColor kWallColor = kGray;

        // left, right, top, bottom
        struct WallDef { core::Vec2F pos; core::Vec2F size; };
        const WallDef walls[] = {
            {{ -kThickness * 0.5f,          screen_size.y * 0.5f }, { kThickness, screen_size.y + kThickness * 2 }},
            {{ screen_size.x + kThickness * 0.5f, screen_size.y * 0.5f }, { kThickness, screen_size.y + kThickness * 2 }},
            {{ screen_size.x * 0.5f,         -kThickness * 0.5f  }, { screen_size.x + kThickness * 2, kThickness }},
            {{ screen_size.x * 0.5f,  screen_size.y + kThickness * 0.5f }, { screen_size.x + kThickness * 2, kThickness }},
        };

        for (auto& w : walls) {
            core::Index<physics::Body> body_idx = physics::AddStaticBody();
            physics::body_at(body_idx).position = w.pos;
            physics::AABB aabb(w.size);
            core::Index<physics::Collider> collider_idx = physics::AddColliderToBody(
                body_idx, {0.f, 0.f}, 1.f, aabb, physics::ShapeType::AABB, false);
            wall_objects_.emplace_back(collider_idx, kWallColor);
        }
    }

    void CollidersSample::SetColor() {
        for (auto &r: rendered_objects_) {
            if (r.collision_count > 0) {
                r.color = kCollisionColor;
            } else if (r.trigger_count > 0) {
                r.color = kTriggerColor;
            } else {
                r.color = kNoOverlapColor;
            }
        }
    }

    void CollidersSample::OnTriggerEnter(physics::CollidersPair cols) {
        for (auto &r: rendered_objects_) {
            if (r.body_idx() == physics::collider_at(cols.collider_idx1).body_idx ||
                r.body_idx() == physics::collider_at(cols.collider_idx2).body_idx) {
                r.trigger_count++;
            }
        }
    }

    void CollidersSample::OnTriggerExit(physics::CollidersPair cols) {
        for (auto &r: rendered_objects_) {
            if (r.body_idx() == physics::collider_at(cols.collider_idx1).body_idx ||
                r.body_idx() == physics::collider_at(cols.collider_idx2).body_idx) {
                if (r.trigger_count > 0) {
                    r.trigger_count--;
                }
            }
        }
    }

    void CollidersSample::OnColliderEnter(physics::CollidersPair cols) {
        for (auto &r: rendered_objects_) {
            if (r.body_idx() == physics::collider_at(cols.collider_idx1).body_idx ||
                r.body_idx() == physics::collider_at(cols.collider_idx2).body_idx) {
                r.collision_count++;
            }
        }
    }

    void CollidersSample::OnColliderExit(physics::CollidersPair cols) {
        for (auto &r: rendered_objects_) {
            if (r.body_idx() == physics::collider_at(cols.collider_idx1).body_idx ||
                r.body_idx() == physics::collider_at(cols.collider_idx2).body_idx) {
                if (r.collision_count > 0) {
                    r.collision_count--;
                }
            }
        }
    }

    void CollidersSample::Draw() {
        if (const auto *renderer = common::GetRenderer(); !renderer) return;
        if (show_quadtree) {
            if (auto *qt = physics::quad_tree()) {
                debug::DrawQuadTree(*qt);
            }
        }
        for (auto &r: wall_objects_) {
            r.Draw();
        }
        for (auto &r: rendered_objects_) {
            r.Draw();
        }
    }
} // namespace colliders
