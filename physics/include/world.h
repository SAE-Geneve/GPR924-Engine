// Created by hugze on 20.10.2025.

#ifndef PHYSICS_INCLUDE_WORLD_H_
#define PHYSICS_INCLUDE_WORLD_H_

#include <memory>
#include <unordered_set>

#include "body.h"
#include "container/indexed_container.h"
#include "quadtree.h"
#include "shapes.h"

namespace listeners {
class ContactListener;
}  // namespace listeners

namespace physics {

struct Collider {
  Collider(core::Index<Body> body_idx, core::Vec2F offset, Shape shape,
           ShapeType shape_type)
      : body_idx(body_idx),
        offset(offset),
        shape(shape),
        shape_type(shape_type),
        collider_idx(-1) {}

  core::Index<Body> body_idx;
  core::Index<Collider> collider_idx;
  core::Vec2F offset = {0.f, 0.f};
  float restitution = 1.f;
  Shape shape;
  ShapeType shape_type = ShapeType::Length;
  bool is_trigger = false;

  // Delegates to the global world instance — defined in world.cc
  [[nodiscard]] Body& body() const;

  bool operator==(const Collider& other) const {
    return body_idx == other.body_idx;
  }

  [[nodiscard]] bool IsInvalid() const {
    return collider_idx == core::Index<Collider>{-1, 0};
  }

  [[nodiscard]] static Collider GenerateInvalidValue() {
    return Collider(core::Index<Body>{-1, 0}, {0.f, 0.f}, Circle(0.f),
                    ShapeType::Circle);
  }
};

struct CollidersPair {
  core::Index<Collider> collider_idx1;
  core::Index<Collider> collider_idx2;

  bool operator==(const CollidersPair& other) const {
    auto a1 = collider_idx1.index(), b1 = collider_idx2.index();
    if (a1 > b1) std::swap(a1, b1);
    auto a2 = other.collider_idx1.index(), b2 = other.collider_idx2.index();
    if (a2 > b2) std::swap(a2, b2);
    return a1 == a2 && b1 == b2;
  }

  bool operator<(const CollidersPair& other) const {
    auto a1 = collider_idx1.index(), b1 = collider_idx2.index();
    if (a1 > b1) std::swap(a1, b1);
    auto a2 = other.collider_idx1.index(), b2 = other.collider_idx2.index();
    if (a2 > b2) std::swap(a2, b2);
    if (a1 != a2) return a1 < a2;
    return b1 < b2;
  }
};

struct ColliderPairHash {
  std::size_t operator()(const CollidersPair& pair) const {
    auto a = pair.collider_idx1.index(), b = pair.collider_idx2.index();
    if (a > b) std::swap(a, b);
    return std::hash<size_t>()(a) ^ (std::hash<size_t>()(b) << 1);
  }
};

struct Contact {
  core::Vec2F contact_point{0.f, 0.f};
  core::Vec2F normal{0.f, 0.f};  // unit, points from A -> B
  float penetration = 0.f;
  core::Index<Collider> collider_idxA{-1, 0};
  core::Index<Collider> collider_idxB{-1, 0};
};

// State is isolated in member variables so it can be snapshotted for rollback.
class PhysicsWorld {
 public:
  void set_bounds(const AABB& bounds);
  const AABB& bounds() const;
  QuadTree* quad_tree() const;
  void set_contact_listener(listeners::ContactListener* listener);

  [[nodiscard]] core::Index<Body> AddBody(float mass);
  [[nodiscard]] core::Index<Body> AddStaticBody();
  [[nodiscard]] Body& body_at(core::Index<Body> idx);
  [[nodiscard]] const Body& body_at(core::Index<Body> idx) const;
  void RemoveBody(core::Index<Body> idx);

  [[nodiscard]] core::Index<Collider> AddCollider(
      core::Index<Body> body_idx, core::Vec2F offset, float restitution,
      Shape shape, ShapeType shape_type, bool is_trigger);
  [[nodiscard]] Collider& collider_at(core::Index<Collider> idx);
  [[nodiscard]] const Collider& collider_at(core::Index<Collider> idx) const;
  void RemoveCollider(core::Index<Collider> idx);

  void Tick(float dt);
  void RollBackFrom(const PhysicsWorld& ref);

 private:
  void CheckForOverlap();
  void ResolveCollisions();
  [[nodiscard]] AABB ColliderToAABB(const Collider& c) const;

  core::IndexedContainer<Body> bodies_;
  core::IndexedContainer<Collider> colliders_;
  AABB bounds_{{0, 0}, {1000, 1000}};
  std::unique_ptr<QuadTree> quadtree_;
  listeners::ContactListener* listener_ = nullptr;

  std::unordered_set<CollidersPair, ColliderPairHash> overlapped_colliders_;
  std::unordered_set<CollidersPair, ColliderPairHash> previously_overlapped_colliders_;
};

PhysicsWorld& world();

void set_world_bounds(const AABB& bounds);
const AABB& world_bounds();
QuadTree* quad_tree();
void set_contact_listener(listeners::ContactListener* listener);

[[nodiscard]] core::Index<Body> AddBody(float mass);
[[nodiscard]] Body& body_at(core::Index<Body> idx);
[[nodiscard]] const Body& const_body_at(core::Index<Body> idx);
void RemoveBody(core::Index<Body> idx);

[[nodiscard]] core::Index<Collider> AddColliderToBody(
    core::Index<Body> body_idx, core::Vec2F offset, float restitution,
    Shape shape, ShapeType shape_type, bool is_trigger);
[[nodiscard]] Collider& collider_at(core::Index<Collider> idx);
[[nodiscard]] const Collider& const_collider_at(core::Index<Collider> idx);
void RemoveCollider(core::Index<Collider> idx);

void Tick(float dt);
void ResetWorld();

}  // namespace physics

#endif  // PHYSICS_INCLUDE_WORLD_H_
