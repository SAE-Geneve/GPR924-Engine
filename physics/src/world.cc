//
// Created by hugze on 20.10.2025.
//

#include <unordered_set>
#include <vector>

#include "collision.h"
#include "contact_listener.h"
#include "maths/geometry.h"

template<typename T>
struct std::hash<core::Index<T>> {
    std::size_t operator()(const core::Index<T> &id) const {
        return std::hash<size_t>()(id.index());
    }
};

namespace physics {
    namespace {
        core::IndexedContainer<Body> bodies;
        core::IndexedContainer<Collider> colliders;

        listeners::ContactListener *listener = nullptr;
        std::unordered_set<CollidersPair, ColliderPairHash> overlapped_colliders_;
        std::unordered_set<CollidersPair, ColliderPairHash> previously_overlapped_colliders_;

        AABB world_bounds{{0, 0}, {1000, 1000}}; // default
        std::unique_ptr<QuadTree> collision_tree = nullptr;
    } // namespace


    void SetWorldBounds(const AABB &b) {
        world_bounds = b;
        collision_tree = std::make_unique<QuadTree>(0, world_bounds);
    }

    const AABB& GetWorldBounds() {
        return world_bounds;
    }

    QuadTree* GetQuadTree() {
        return collision_tree.get();
    }

    void set_contact_listener(listeners::ContactListener *listener_) {
        listener = listener_;
    }

    [[nodiscard]] core::Index<Collider> AddColliderToBody(
        core::Index<Body> body_idx, core::Vec2F offset, float restitution, Shape shape, ShapeType shape_type,
        bool isTrigger) {
        Collider collider(body_idx, offset, shape, shape_type);
        core::Index<Collider> const idx = colliders.Add(collider);
        colliders.At(idx).collider_idx = idx;
        colliders.At(idx).is_trigger = isTrigger;
        colliders.At(idx).restitution = restitution;
        return idx;
    }

    [[nodiscard]] Collider &get_collider_at(core::Index<Collider> idx) {
        return colliders.At(idx);
    }

    void RemoveCollider(core::Index<Collider> idx) {
        // Fire exit events for any active overlaps involving this collider
        if (listener) {
            for (auto &pair : overlapped_colliders_) {
                if (pair.collider_idx1 == idx || pair.collider_idx2 == idx) {
                    Collider &c1 = get_collider_at(pair.collider_idx1);
                    Collider &c2 = get_collider_at(pair.collider_idx2);
                    if (c1.is_trigger || c2.is_trigger)
                        listener->OnTriggerExit(pair);
                    else
                        listener->OnColliderExit(pair);
                }
            }
        }

        auto remove_from = [&idx](auto &set) {
            for (auto it = set.begin(); it != set.end();) {
                if (it->collider_idx1 == idx || it->collider_idx2 == idx)
                    it = set.erase(it);
                else
                    ++it;
            }
        };
        remove_from(overlapped_colliders_);
        remove_from(previously_overlapped_colliders_);

        colliders.Remove(idx);
    }

    [[nodiscard]] core::Index<Body> AddBody(const float mass) {
        return bodies.Add(Body(mass));
    }

    [[nodiscard]] Body &get_body_at(core::Index<Body> idx) {
        return bodies.At(idx);
    }

    void RemoveBody(core::Index<Body> idx) { bodies.Remove(idx); }

    [[nodiscard]] AABB ShapeToAABB(const Collider &c) {
        switch (c.shape_type) {
            case ShapeType::AABB: {
                const auto &box = std::get<AABB>(c.shape);
                core::Vec2F center = c.body().position + c.offset;
                return AABB{
                    center + box.lower_bound,
                    center + box.upper_bound
                };
            }

            case ShapeType::Circle: {
                const auto &circle = std::get<Circle>(c.shape);
                float r = circle.radius;
                core::Vec2F center = c.body().position + c.offset;
                return AABB{
                    {center.x - r, center.y - r},
                    {center.x + r, center.y + r}
                };
            }

            default:
                return AABB{{0, 0}, {0, 0}};
        }
    }

    void CheckForOverlapWithQuadTree() {
        if (!listener) return;
        if (!collision_tree) return;

        previously_overlapped_colliders_ = overlapped_colliders_;
        overlapped_colliders_.clear();

        collision_tree->clear();

        // Insert all colliders into quadtree
        for (auto &c: colliders) {
            if (c.IsInvalid()) continue;
            collision_tree->insert({c.collider_idx, ShapeToAABB(c)});
        }

        std::vector<CollidersPair> candidates;
        collision_tree->query(candidates);

        for (auto &pair: candidates) {
            auto &c1 = get_collider_at(pair.collider_idx1);
            auto &c2 = get_collider_at(pair.collider_idx2);
            const core::Vec2F pos1 = c1.body().position + c1.offset;
            const core::Vec2F pos2 = c2.body().position + c2.offset;

            bool overlap = false;
            switch (c1.shape_type) {
                case ShapeType::Circle:
                    switch (c2.shape_type) {
                        case ShapeType::Circle:
                            overlap = core::CircleOverlap(
                                pos1, std::get<Circle>(c1.shape).radius,
                                pos2, std::get<Circle>(c2.shape).radius
                            );
                            break;
                        case ShapeType::AABB:
                            overlap = core::AABBCircleOverlap(
                                pos2,
                                {std::get<AABB>(c2.shape).width(), std::get<AABB>(c2.shape).height()},
                                pos1, std::get<Circle>(c1.shape).radius
                            );
                            break;
                        default: ;
                    }
                    break;
                case ShapeType::AABB:
                    switch (c2.shape_type) {
                        case ShapeType::Circle:
                            overlap = core::AABBCircleOverlap(
                                pos1,
                                {std::get<AABB>(c1.shape).width(), std::get<AABB>(c1.shape).height()},
                                pos2, std::get<Circle>(c2.shape).radius
                            );
                            break;
                        case ShapeType::AABB:
                            overlap = core::AABBOverlap(
                                pos1,
                                {std::get<AABB>(c1.shape).width(), std::get<AABB>(c1.shape).height()},
                                pos2,
                                {std::get<AABB>(c2.shape).width(), std::get<AABB>(c2.shape).height()}
                            );
                            break;
                        default: ;
                    }
                    break;
                default: ;
            }

            // Enter event
            if (overlap) {
                const bool is_new = overlapped_colliders_.emplace(pair).second;

                if (is_new && !previously_overlapped_colliders_.contains(pair)) {
                    if (c1.is_trigger || c2.is_trigger)
                        listener->OnTriggerEnter(pair);
                    else
                        listener->OnColliderEnter(pair);
                }
            }
        }

        // Exit event
        for (auto &p : previously_overlapped_colliders_) {
            if (!overlapped_colliders_.contains(p)) {
                Collider &c1 = get_collider_at(p.collider_idx1);
                Collider &c2 = get_collider_at(p.collider_idx2);

                if (c1.is_trigger || c2.is_trigger)
                    listener->OnTriggerExit(p);
                else
                    listener->OnColliderExit(p);
            }
        }
    }

    void ResolveCollisions() {
        for (auto &overlapped_collider: overlapped_colliders_) {
            core::Index<Collider> idx1 = overlapped_collider.collider_idx1;
            core::Index<Collider> idx2 = overlapped_collider.collider_idx2;

            Collider &c1 = get_collider_at(idx1);
            Collider &c2 = get_collider_at(idx2);

            if (!c1.is_trigger && !c2.is_trigger) {
                if (auto maybeContact = GenerateContact(idx1, idx2)) {
                    ResolveCollision(*maybeContact, (c1.restitution + c2.restitution) / 2);
                }
            }
        }
    }

    void Tick(const float dt) {
        // Advance bodies
        for (auto &body: bodies) {
            if (body.IsInvalid()) continue;
            body.Tick(dt);
        }

        // BroadPhase
        CheckForOverlapWithQuadTree();

        // NarrowPhase
        ResolveCollisions();
    }
} // namespace physics
