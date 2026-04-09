// Created by hugze on 20.10.2025.

#include "world.h"

#include <algorithm>
#include <unordered_set>
#include <vector>

#include "collision.h"
#include "contact_listener.h"
#include "maths/geometry.h"

template <>
struct std::hash<core::Index<physics::Collider>> {
  std::size_t operator()(const core::Index<physics::Collider>& id) const {
    return std::hash<size_t>()(id.index());
  }
};

namespace physics {

static PhysicsWorld g_world;
static PhysicsWorld* active_world_ = &g_world;

PhysicsWorld& world() { return g_world; }

Body& Collider::body() const { return active_world_->body_at(body_idx); }

void PhysicsWorld::set_bounds(const AABB& b) {
  bounds_ = b;
  quadtree_ = std::make_unique<QuadTree>(0, bounds_);
}

const AABB& PhysicsWorld::bounds() const { return bounds_; }

QuadTree* PhysicsWorld::quad_tree() const { return quadtree_.get(); }

void PhysicsWorld::set_contact_listener(listeners::ContactListener* l) {
  listener_ = l;
}

core::Index<Body> PhysicsWorld::AddBody(float mass) {
  return bodies_.Add(Body(mass));
}

core::Index<Body> PhysicsWorld::AddStaticBody() {
  core::Index<Body> idx = AddBody(0.f);
  bodies_.At(idx).is_static = true;
  return idx;
}

Body& PhysicsWorld::body_at(core::Index<Body> idx) {
  return bodies_.At(idx);
}

const Body& PhysicsWorld::body_at(core::Index<Body> idx) const {
  return bodies_.At(idx);
}

void PhysicsWorld::RemoveBody(core::Index<Body> idx) { bodies_.Remove(idx); }

core::Index<Collider> PhysicsWorld::AddCollider(
    core::Index<Body> body_idx, core::Vec2F offset, float restitution,
    Shape shape, ShapeType shape_type, bool is_trigger) {
  Collider collider(body_idx, offset, shape, shape_type);
  core::Index<Collider> idx = colliders_.Add(collider);
  colliders_.At(idx).collider_idx = idx;
  colliders_.At(idx).is_trigger = is_trigger;
  colliders_.At(idx).restitution = restitution;
  return idx;
}

Collider& PhysicsWorld::collider_at(core::Index<Collider> idx) {
  return colliders_.At(idx);
}

const Collider& PhysicsWorld::collider_at(core::Index<Collider> idx) const {
  return colliders_.At(idx);
}

void PhysicsWorld::RemoveCollider(core::Index<Collider> idx) {
  if (listener_) {
    std::vector<CollidersPair> sorted_overlaps(overlapped_colliders_.begin(),
                                               overlapped_colliders_.end());
    std::sort(sorted_overlaps.begin(), sorted_overlaps.end());
    for (auto& pair : sorted_overlaps) {
      if (pair.collider_idx1 == idx || pair.collider_idx2 == idx) {
        Collider& c1 = colliders_.At(pair.collider_idx1);
        Collider& c2 = colliders_.At(pair.collider_idx2);
        if (c1.is_trigger || c2.is_trigger)
          listener_->OnTriggerExit(pair);
        else
          listener_->OnColliderExit(pair);
      }
    }
  }

  auto remove_from = [&idx](auto& set) {
    for (auto it = set.begin(); it != set.end();) {
      if (it->collider_idx1 == idx || it->collider_idx2 == idx)
        it = set.erase(it);
      else
        ++it;
    }
  };
  remove_from(overlapped_colliders_);
  remove_from(previously_overlapped_colliders_);

  colliders_.Remove(idx);
}

AABB PhysicsWorld::ColliderToAABB(const Collider& c) const {
  switch (c.shape_type) {
    case ShapeType::AABB: {
      const auto& box = std::get<AABB>(c.shape);
      core::Vec2F center = bodies_.At(c.body_idx).position + c.offset;
      return AABB{center + box.lower_bound, center + box.upper_bound};
    }
    case ShapeType::Circle: {
      float r = std::get<Circle>(c.shape).radius;
      core::Vec2F center = bodies_.At(c.body_idx).position + c.offset;
      return AABB{{center.x - r, center.y - r}, {center.x + r, center.y + r}};
    }
    default:
      return AABB{{0, 0}, {0, 0}};
  }
}

void PhysicsWorld::CheckForOverlap() {
  if (!listener_) return;
  if (!quadtree_) return;

  previously_overlapped_colliders_ = overlapped_colliders_;
  overlapped_colliders_.clear();
  quadtree_->clear();

  for (auto& c : colliders_) {
    if (c.IsInvalid()) continue;
    quadtree_->insert({c.collider_idx, ColliderToAABB(c)});
  }

  std::vector<CollidersPair> candidates;
  quadtree_->query(candidates);

  for (auto& pair : candidates) {
    auto& c1 = colliders_.At(pair.collider_idx1);
    auto& c2 = colliders_.At(pair.collider_idx2);

    const core::Vec2F pos1 = bodies_.At(c1.body_idx).position + c1.offset;
    const core::Vec2F pos2 = bodies_.At(c2.body_idx).position + c2.offset;

    bool overlap = false;
    switch (c1.shape_type) {
      case ShapeType::Circle:
        switch (c2.shape_type) {
          case ShapeType::Circle:
            overlap = core::CircleOverlap(pos1,
                                          std::get<Circle>(c1.shape).radius,
                                          pos2,
                                          std::get<Circle>(c2.shape).radius);
            break;
          case ShapeType::AABB:
            overlap = core::AABBCircleOverlap(
                pos2,
                {std::get<AABB>(c2.shape).width(),
                 std::get<AABB>(c2.shape).height()},
                pos1, std::get<Circle>(c1.shape).radius);
            break;
          default:;
        }
        break;
      case ShapeType::AABB:
        switch (c2.shape_type) {
          case ShapeType::Circle:
            overlap = core::AABBCircleOverlap(
                pos1,
                {std::get<AABB>(c1.shape).width(),
                 std::get<AABB>(c1.shape).height()},
                pos2, std::get<Circle>(c2.shape).radius);
            break;
          case ShapeType::AABB:
            overlap = core::AABBOverlap(
                pos1,
                {std::get<AABB>(c1.shape).width(),
                 std::get<AABB>(c1.shape).height()},
                pos2,
                {std::get<AABB>(c2.shape).width(),
                 std::get<AABB>(c2.shape).height()});
            break;
          default:;
        }
        break;
      default:;
    }

    if (overlap) {
      const bool is_new = overlapped_colliders_.emplace(pair).second;
      if (is_new && !previously_overlapped_colliders_.contains(pair)) {
        if (c1.is_trigger || c2.is_trigger)
          listener_->OnTriggerEnter(pair);
        else
          listener_->OnColliderEnter(pair);
      }
    }
  }

  std::vector<CollidersPair> sorted_prev(previously_overlapped_colliders_.begin(),
                                         previously_overlapped_colliders_.end());
  std::sort(sorted_prev.begin(), sorted_prev.end());
  for (auto& p : sorted_prev) {
    if (!overlapped_colliders_.contains(p)) {
      Collider& c1 = colliders_.At(p.collider_idx1);
      Collider& c2 = colliders_.At(p.collider_idx2);
      if (c1.is_trigger || c2.is_trigger)
        listener_->OnTriggerExit(p);
      else
        listener_->OnColliderExit(p);
    }
  }
}

void PhysicsWorld::ResolveCollisions() {
  std::vector<CollidersPair> sorted_overlaps(overlapped_colliders_.begin(),
                                             overlapped_colliders_.end());
  std::sort(sorted_overlaps.begin(), sorted_overlaps.end());
  for (auto& pair : sorted_overlaps) {
    Collider& c1 = colliders_.At(pair.collider_idx1);
    Collider& c2 = colliders_.At(pair.collider_idx2);
    if (!c1.is_trigger && !c2.is_trigger) {
      if (auto contact =
              GenerateContact(pair.collider_idx1, pair.collider_idx2)) {
        ResolveCollision(*contact, (c1.restitution + c2.restitution) / 2.f);
      }
    }
  }
}

void PhysicsWorld::RollBackFrom(const PhysicsWorld& ref) {
  bodies_ = ref.bodies_;
  colliders_ = ref.colliders_;
  overlapped_colliders_ = ref.overlapped_colliders_;
  previously_overlapped_colliders_ = ref.previously_overlapped_colliders_;
  bounds_ = ref.bounds_;
  quadtree_ = ref.quadtree_ ? std::make_unique<QuadTree>(0, bounds_) : nullptr;
}

void PhysicsWorld::Tick(float dt) {
  auto* prev = active_world_;
  active_world_ = this;
  for (auto& body : bodies_) {
    if (body.IsInvalid()) continue;
    body.Tick(dt);
  }
  CheckForOverlap();
  ResolveCollisions();
  active_world_ = prev;
}

void set_world_bounds(const AABB& b) { g_world.set_bounds(b); }
const AABB& world_bounds() { return g_world.bounds(); }
QuadTree* quad_tree() { return g_world.quad_tree(); }
void set_contact_listener(listeners::ContactListener* l) {
  g_world.set_contact_listener(l);
}

core::Index<Body> AddBody(float mass) { return g_world.AddBody(mass); }

Body& body_at(core::Index<Body> idx) { return g_world.body_at(idx); }
const Body& const_body_at(core::Index<Body> idx) {
  return const_cast<const PhysicsWorld&>(g_world).body_at(idx);
}
void RemoveBody(core::Index<Body> idx) { g_world.RemoveBody(idx); }

core::Index<Collider> AddColliderToBody(core::Index<Body> body_idx,
                                        core::Vec2F offset, float restitution,
                                        Shape shape, ShapeType shape_type,
                                        bool is_trigger) {
  return g_world.AddCollider(body_idx, offset, restitution, shape, shape_type,
                             is_trigger);
}
Collider& collider_at(core::Index<Collider> idx) {
  return active_world_->collider_at(idx);
}
const Collider& const_collider_at(core::Index<Collider> idx) {
  return const_cast<const PhysicsWorld*>(active_world_)->collider_at(idx);
}
void RemoveCollider(core::Index<Collider> idx) { g_world.RemoveCollider(idx); }

void Tick(float dt) { g_world.Tick(dt); }
void ResetWorld() { g_world = PhysicsWorld{}; }

}  // namespace physics
