//
// Created by hugze on 16.11.2025.
//

#ifndef QUAD_TREE_H
#define QUAD_TREE_H
#include <array>
#include <memory>

#include "shapes.h"
#include "container/indexed_container.h"


namespace physics {
    struct CollidersPair;
    struct Collider;
}

namespace physics {
    struct QTEntry {
        core::Index<Collider> idx;
        AABB box;
    };

    class QuadTree {
    public:
        static constexpr int MAX_OBJECTS = 8; // Max number of object per levels
        static constexpr int MAX_LEVELS = 5; // Max number of levels

        QuadTree(int level, const AABB &bounds)
            : level_(level), bounds_(bounds) {
        }

        // Insert collider
        void insert(const QTEntry &entry);

        // Query candidates (stores results in 'out')
        void query(std::vector<CollidersPair> &out) const;

        void clear();

        [[nodiscard]] const AABB& bounds() const { return bounds_; }
        [[nodiscard]] const std::array<std::unique_ptr<QuadTree>, 4>& children() const { return nodes_; }
        [[nodiscard]] const std::vector<QTEntry>& objects() const { return objects_; }

    private:
        int level_;
        AABB bounds_;
        std::vector<QTEntry> objects_;
        std::array<std::unique_ptr<QuadTree>, 4> nodes_;


        void subdivide();

        [[nodiscard]] int getChildIndex(const AABB &box) const;

        void objectQuery(QTEntry object, std::vector<CollidersPair> &out) const;

        void checkSiblingCollision(const QuadTree &a, const QuadTree &b,
                                     std::vector<CollidersPair> &out) const;
    };
}
#endif //QUAD_TREE_H
