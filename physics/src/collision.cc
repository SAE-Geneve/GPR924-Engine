//
// Created by hugze on 13.11.2025.
//

#include "collision.h"
#include "world.h"
#include <cmath>
#include <algorithm>

namespace physics {

static std::optional<Contact> CircleCircleContact(core::Index<Collider> a,
                                                  core::Index<Collider> b) {
  auto& cA = collider_at(a);
  auto& cB = collider_at(b);
  core::Vec2F posA = cA.body().position + cA.offset;
  core::Vec2F posB = cB.body().position + cB.offset;
  float rA = std::get<Circle>(cA.shape).radius;
  float rB = std::get<Circle>(cB.shape).radius;

  core::Vec2F diff = posB - posA;
  float dist2 = diff.Dot(diff);
  float rSum = rA + rB;
  if (dist2 >= rSum * rSum) return std::nullopt;

  float dist = std::sqrt(dist2);
  core::Vec2F normal = (dist > 0.0f) ? diff / dist : core::Vec2F{1.f, 0.f};
  float penetration = rSum - dist;

  Contact contact;
  contact.collider_idxA = a;
  contact.collider_idxB = b;
  contact.normal = normal; // A -> B
  contact.penetration = penetration;
  contact.contact_point = posA + normal * (rA - penetration * 0.5f);
  // Ensure normal always goes from A → B
  core::Vec2F AB = (collider_at(contact.collider_idxB).body().position + collider_at(contact.collider_idxB).offset)
                 - (collider_at(contact.collider_idxA).body().position + collider_at(contact.collider_idxA).offset);
  if (contact.normal.Dot(AB) < 0.0f)
    contact.normal = -contact.normal;
  return contact;
}

static std::optional<Contact> AABB_AABB_Contact(core::Index<Collider> a,
                                                core::Index<Collider> b) {
  auto& cA = collider_at(a);
  auto& cB = collider_at(b);
  core::Vec2F posA = cA.body().position + cA.offset;
  core::Vec2F posB = cB.body().position + cB.offset;
  core::Vec2F aSize{std::get<AABB>(cA.shape).width(), std::get<AABB>(cA.shape).height()};
  core::Vec2F bSize{std::get<AABB>(cB.shape).width(), std::get<AABB>(cB.shape).height()};

  float dx = posB.x - posA.x;
  float px = (aSize.x/2 + bSize.x/2) - std::abs(dx);
  if (px <= 0) return std::nullopt;
  float dy = posB.y - posA.y;
  float py = (aSize.y/2 + bSize.y/2) - std::abs(dy);
  if (py <= 0) return std::nullopt;

  Contact contact;
  contact.collider_idxA = a;
  contact.collider_idxB = b;

  if (px < py) {
    contact.penetration = px;
    contact.normal = (dx < 0) ? core::Vec2F{-1.f, 0.f} : core::Vec2F{1.f, 0.f}; // A -> B
    float cx = posA.x + (aSize.x/2) * contact.normal.x;
    float cy = (posA.y + posB.y) * 0.5f;
    contact.contact_point = core::Vec2F{cx, cy};
  } else {
    contact.penetration = py;
    contact.normal = (dy < 0) ? core::Vec2F{0.f, -1.f} : core::Vec2F{0.f, 1.f};
    float cy = posA.y + (aSize.y/2) * contact.normal.y;
    float cx = (posA.x + posB.x) * 0.5f;
    contact.contact_point = core::Vec2F{cx, cy};
  }
  // Ensure normal always goes from A → B
  core::Vec2F AB = (collider_at(contact.collider_idxB).body().position + collider_at(contact.collider_idxB).offset)
                 - (collider_at(contact.collider_idxA).body().position + collider_at(contact.collider_idxA).offset);
  if (contact.normal.Dot(AB) < 0.0f)
    contact.normal = -contact.normal;
  return contact;
}

static std::optional<Contact> CircleAABBContact(core::Index<Collider> circIdx,
                                                core::Index<Collider> aabbIdx,
                                                bool circleIsA) {
  auto& cC = collider_at(circIdx);
  auto& cR = collider_at(aabbIdx);

  core::Vec2F circlePos = cC.body().position + cC.offset;
  core::Vec2F aabbPos = cR.body().position + cR.offset;
  core::Vec2F halfSize{std::get<AABB>(cR.shape).width() * 0.5f,
                 std::get<AABB>(cR.shape).height() * 0.5f};

  core::Vec2F local = circlePos - aabbPos;
  core::Vec2F closest = local;
  closest.x = std::clamp(closest.x, -halfSize.x, halfSize.x);
  closest.y = std::clamp(closest.y, -halfSize.y, halfSize.y);

  core::Vec2F diff = local - closest;
  float dist2 = diff.Dot(diff);
  float r = std::get<Circle>(cC.shape).radius;

  if (dist2 > r * r) return std::nullopt;

  float dist = std::sqrt(dist2);
  Contact contact;
  if (dist > 0.0f) {
    core::Vec2F normal_local = diff / dist; // points from closest point -> circle center
    // If circle is A and aabb is B, normal must point A -> B (circle -> box) == -normal_local
    contact.normal = circleIsA ? -normal_local : normal_local;
  } else {
    core::Vec2F n = (circlePos - aabbPos);
    float len = std::sqrt(n.Dot(n));
    contact.normal = (len > 0.0f) ? (n / len) : core::Vec2F{1.f, 0.f};
    if (!circleIsA) contact.normal = -contact.normal;
  }

  contact.collider_idxA = circleIsA ? circIdx : aabbIdx;
  contact.collider_idxB = circleIsA ? aabbIdx : circIdx;
  contact.penetration = r - dist;
  contact.contact_point = aabbPos + closest;
  // Ensure normal always goes from A → B
  core::Vec2F AB = (collider_at(contact.collider_idxB).body().position + collider_at(contact.collider_idxB).offset)
                 - (collider_at(contact.collider_idxA).body().position + collider_at(contact.collider_idxA).offset);
  if (contact.normal.Dot(AB) < 0.0f)
    contact.normal = -contact.normal;
  return contact;
}

std::optional<Contact> GenerateContact(core::Index<Collider> idxA,
                                       core::Index<Collider> idxB) {
  auto& cA = collider_at(idxA);
  auto& cB = collider_at(idxB);

  if (cA.shape_type == ShapeType::Circle && cB.shape_type == ShapeType::Circle)
    return CircleCircleContact(idxA, idxB);

  if (cA.shape_type == ShapeType::AABB && cB.shape_type == ShapeType::AABB)
    return AABB_AABB_Contact(idxA, idxB);

  if (cA.shape_type == ShapeType::Circle && cB.shape_type == ShapeType::AABB)
    return CircleAABBContact(idxA, idxB, true);

  if (cA.shape_type == ShapeType::AABB && cB.shape_type == ShapeType::Circle)
    return CircleAABBContact(idxB, idxA, false);

  return std::nullopt;
}

void ResolveCollision(Contact& contact, float restitution) {
  auto& colA = collider_at(contact.collider_idxA);
  auto& colB = collider_at(contact.collider_idxB);
  Body& A = colA.body();
  Body& B = colB.body();

  core::Vec2F rv = B.velocity() - A.velocity();
  float velAlongNormal = rv.Dot(contact.normal);

  if (velAlongNormal > 0.0f) {
    // moving apart
    return;
  }

  float invMassA = A.is_static ? 0.f : 1.f / A.mass();
  float invMassB = B.is_static ? 0.f : 1.f / B.mass();
  float denom = invMassA + invMassB;
  if (denom <= 0.0f) return; // both static

  float j = -(1.0f + restitution) * velAlongNormal;
  j /= denom;

  core::Vec2F impulse = contact.normal * j;
  A.Velocity(A.velocity() - impulse * invMassA);
  B.Velocity(B.velocity() + impulse * invMassB);

  // positional correction
  const float percent = 0.2f;
  const float slop = 0.01f;
  float correctionMag = std::max(contact.penetration - slop, 0.0f) / denom * percent;
  core::Vec2F correction = contact.normal * correctionMag;
  A.position -= correction * invMassA;
  B.position += correction * invMassB;
}

} // namespace physics