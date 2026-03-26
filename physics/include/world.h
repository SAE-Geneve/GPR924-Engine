//
// Created by hugze on 20.10.2025.
//

#ifndef WORLD_H
#define WORLD_H

#include "body.h"
#include "container/indexed_container.h"
#include "shapes.h"
#include "quadtree.h"

namespace listeners {
    class ContactListener;
}

namespace physics {
    void SetWorldBounds(const AABB& bounds);

    const AABB& GetWorldBounds();

    QuadTree* GetQuadTree();

    void set_contact_listener(listeners::ContactListener *listener);

    [[nodiscard]] core::Index<Body> AddBody(float mass);

    [[nodiscard]] Body &get_body_at(core::Index<Body> idx);

    void RemoveBody(core::Index<Body> idx);

    struct Collider {
        Collider(core::Index<Body> body_idx_, core::Vec2F offset_, Shape shape,
                 ShapeType shape_type_)
            : body_idx(body_idx_),
              offset(offset_),
              shape(shape),
              shape_type(shape_type_),
              collider_idx(-1) {
        }


        core::Index<Body> body_idx;
        core::Index<Collider> collider_idx;
        core::Vec2F offset = {0.f, 0.f};
        float restitution = 1.f;
        Shape shape;
        ShapeType shape_type = ShapeType::Length;
        bool is_trigger = false;

        [[nodiscard]] Body &body() const { return get_body_at(body_idx); }


        bool operator==(const Collider &other) const {
            return (body_idx == other.body_idx);
        }

        [[nodiscard]] bool IsInvalid() const {
            return collider_idx == core::Index<Collider>{-1, 0};
        }

        [[nodiscard]] static Collider GenerateInvalidValue() {
            Collider col(core::Index<Body>{-1, 0}, {0.f, 0.f}, Circle(0.f), ShapeType::Circle);
            return col;
        }
    };

    struct CollidersPair {
        core::Index<Collider> collider_idx1;
        core::Index<Collider> collider_idx2;

        bool operator==(const CollidersPair &other) const {
            auto a1 = collider_idx1.index();
            auto b1 = collider_idx2.index();
            if (a1 > b1) std::swap(a1, b1);

            auto a2 = other.collider_idx1.index();
            auto b2 = other.collider_idx2.index();
            if (a2 > b2) std::swap(a2, b2);

            return a1 == a2 && b1 == b2;
        }
    };

    struct ColliderPairHash {
        std::size_t operator()(const CollidersPair &pair) const {
            auto a = pair.collider_idx1.index();
            auto b = pair.collider_idx2.index();
            if (a > b) std::swap(a, b); // canonical order
            return std::hash<size_t>()(a) ^ (std::hash<size_t>()(b) << 1); // combine safely
        }
    };

    struct Contact {
        core::Vec2F contact_point{0.f, 0.f};
        core::Vec2F normal{0.f, 0.f}; // unit, points from A -> B
        float penetration = 0.f;
        core::Index<Collider> collider_idxA{-1, 0};
        core::Index<Collider> collider_idxB{-1, 0};
    };

    [[nodiscard]] core::Index<Collider> AddColliderToBody(
        core::Index<Body> body_idx, core::Vec2F offset, float restitution, Shape shape, ShapeType shape_type,
        bool isTrigger);

    [[nodiscard]] Collider &get_collider_at(core::Index<Collider> idx);

    void RemoveCollider(core::Index<Collider> idx);

    void CheckForOverlapWithQuadTree();

    void ResolveCollisions();

    void Tick(float dt);
} // namespace physics

#endif  // WORLD_H
