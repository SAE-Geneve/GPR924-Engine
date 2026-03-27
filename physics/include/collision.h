//
// Created by hugze on 13.11.2025.
//

#ifndef COLLISION_H
#define COLLISION_H

#include <optional>
#include "world.h"

namespace physics {

std::optional<Contact> GenerateContact(core::Index<Collider> idxA,
                                       core::Index<Collider> idxB);

void ResolveCollision(Contact& contact, float restitution = 1.f);

}  // namespace physics

#endif  // COLLISION_H
