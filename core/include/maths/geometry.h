#ifndef CORE_MATH_GEOMETRY_H
#define CORE_MATH_GEOMETRY_H
#include <algorithm>

#include "vec2.h"

/*
Copyright 2025 SAE Institute Switzerland SA

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Contributors: Jemoel
*/

namespace core {
constexpr bool CircleContainsPoint(const Vec2F& point, const Vec2F& center,
                                   const float radius) {
  const Vec2F diff = point - center;
  return diff.magnitude_sqr() <= radius * radius;
}
constexpr bool CircleOverlap(const Vec2F& center1, const float radius1,
                             const Vec2F& center2, const float radius2) {
  const Vec2F diff = center1 - center2;
  const float radiusSum = radius1 + radius2;
  return diff.magnitude_sqr() <= radiusSum * radiusSum;
}

constexpr bool AABBOverlap(Vec2F pos1, Vec2F size1, Vec2F pos2, Vec2F size2) {
  // Half sizes of both AABBs
  float halfWidth1 = size1.x * 0.5f;
  float halfHeight1 = size1.y * 0.5f;
  float halfWidth2 = size2.x * 0.5f;
  float halfHeight2 = size2.x * 0.5f;

  // Difference between centers
  float dx = pos1.x - pos2.x;
  if (dx < 0) {
    dx *= -1;
  }

  float dy = pos1.y - pos2.y;
  if (dy < 0) {
    dy *= -1;
  }

  // Check for overlap on both axes
  bool overlapX = dx <= (halfWidth1 + halfWidth2);
  bool overlapY = dy <= (halfHeight1 + halfHeight2);

  return overlapX && overlapY;
}

constexpr bool AABBCircleOverlap(const Vec2F& rectCenter, Vec2F rectSize,
                                 const Vec2F& circleCenter, float radius) {
  // Half extents of the AABB
  float halfWidth = rectSize.x * 0.5f;
  float halfHeight = rectSize.y * 0.5f;

  // Compute the vector from AABB center to circle center
  core::Vec2F diff = circleCenter - rectCenter;

  // Clamp that vector to the box's extents (find the closest point on box)
  float clampedX = std::clamp(diff.x, -halfWidth, halfWidth);
  float clampedY = std::clamp(diff.y, -halfHeight, halfHeight);

  // Closest point on the AABB to the circle center
  core::Vec2F closestPoint = rectCenter + core::Vec2F(clampedX, clampedY);

  // Distance squared between circle center and closest point
  core::Vec2F delta = circleCenter - closestPoint;
  float distanceSqr = delta.magnitude_sqr();

  // Overlap if distance ≤ radius
  return distanceSqr <= (radius * radius);
}
}  // namespace core

#endif  // CORE_MATH_GEOMETRY_H
