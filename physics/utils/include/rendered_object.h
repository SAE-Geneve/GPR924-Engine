//
// Created by hugze on 14.11.2025.
//

#ifndef RENDERED_OBJECT_H
#define RENDERED_OBJECT_H
#include "world.h"
#include "container/indexed_container.h"
#include "engine/renderer.h"

namespace utils {
    struct RenderedObject : common::DrawInterface {
        core::Index<physics::Collider> collider_idx{-1, 0};
        SDL_FColor color;
        int collision_count = 0;
        int trigger_count = 0;

        RenderedObject(core::Index<physics::Collider> collider_idx_,
                       SDL_FColor color_)
            : collider_idx(collider_idx_), color(color_) {}

        [[nodiscard]] physics::Collider& collider() const {
            return get_collider_at(collider_idx);
        }
        [[nodiscard]] physics::Body& body() const { return collider().body(); }
        [[nodiscard]] physics::Circle& circle() const {
            return std::get<physics::Circle>(collider().shape);
        }
        [[nodiscard]] physics::AABB& aabb() const {
            return std::get<physics::AABB>(collider().shape);
        }
        [[nodiscard]] core::Index<physics::Body> body_idx() const {
            return collider().body_idx;
        }

        bool operator==(const RenderedObject& other) const {
            return collider_idx == other.collider_idx;
        }

        void Draw() override {
            if (const auto* renderer = common::GetRenderer(); !renderer) return;

            switch (collider().shape_type) {
                case physics::ShapeType::Circle:
                    common::DrawCircle(body().position.x, body().position.y,
                                       circle().radius, color);
                    break;
                case physics::ShapeType::AABB:
                    common::DrawAABB(body().position, {aabb().width(), aabb().height()},
                                     color);

                    break;
                default:
                    break;
            }
        }
    };
}

#endif //RENDERED_OBJECT_H
