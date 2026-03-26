//
// Created by hugze on 16.11.2025.
//

#ifndef SHAPES_H
#define SHAPES_H
#include <variant>
#include "maths/vec2.h"

namespace physics {
    enum class ShapeType { Circle, AABB, Length };

    struct Circle {
        float radius = 0.f;

        explicit Circle(float radius) : radius(radius) {
        }
    };

    struct AABB {
        core::Vec2F lower_bound = {0, 0};
        core::Vec2F upper_bound = {0, 0};

        explicit AABB(core::Vec2F size) {
            lower_bound = {-(size.x / 2), -(size.y / 2)};
            upper_bound = {(size.x / 2), (size.y / 2)};
        }

        explicit AABB(core::Vec2F lower, core::Vec2F upper) {
            lower_bound = lower;
            upper_bound = upper;
        }

        [[nodiscard]] core::Vec2F center() const {
            return core::Vec2F{
                (lower_bound.x + upper_bound.x) / 2,
                (lower_bound.y + upper_bound.y) / 2
            };
        }

        [[nodiscard]] float height() const { return upper_bound.y - lower_bound.y; }
        [[nodiscard]] float width() const { return upper_bound.x - lower_bound.x; }

        [[nodiscard]] bool intersects(const AABB &other) const {
            return !(upper_bound.x < other.lower_bound.x ||
                     lower_bound.x > other.upper_bound.x ||
                     upper_bound.y < other.lower_bound.y ||
                     lower_bound.y > other.upper_bound.y);
        }

        [[nodiscard]] bool contains(const AABB &other) const {
            return (other.lower_bound.x >= lower_bound.x &&
                    other.upper_bound.x <= upper_bound.x &&
                    other.lower_bound.y >= lower_bound.y &&
                    other.upper_bound.y <= upper_bound.y);
        }
    };

    using Shape = std::variant<Circle, AABB>;
}
#endif //SHAPES_H
