//
// Created by hugze on 16.11.2025.
//

#include "quadtree.h"
#include "world.h"

namespace physics {

    void QuadTree::clear() {
        objects_.clear();
        for (auto &n: nodes_) {
            if (n) n->clear();
            n.reset();
        }
    }

    void QuadTree::insert(const QTEntry &entry) {
        // if subdivided, try to pass to a child
        if (nodes_[0]) {
            int index = getChildIndex(entry.box);
            if (index != -1) {
                nodes_[index]->insert(entry);
                return;
            }
        }

        objects_.push_back(entry);

        // subdivide if needed
        if (objects_.size() > MAX_OBJECTS && level_ < MAX_LEVELS) {
            if (!nodes_[0]) subdivide();

            auto it = objects_.begin();
            while (it != objects_.end()) {
                int index = getChildIndex(it->box);
                if (index != -1) {
                    nodes_[index]->insert(*it);
                    it = objects_.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    void QuadTree::query(std::vector<CollidersPair> &out) const {
        // add local objects
        for (size_t i = 0; i < objects_.size(); ++i) {
            for (size_t j = i+1; j < objects_.size(); ++j) {
                if (objects_[i].box.intersects(objects_[j].box)) {
                    out.emplace_back(objects_[i].idx, objects_[j].idx);
                }
            }
        }

        // query child nodes
        if (nodes_[0]) {
           for (auto &obj: objects_) {
               for (int i = 0; i < 4; ++i) {
                   nodes_[i]->objectQuery(obj, out);
               }
           }

            for (int i = 0; i < 4; ++i) {
                nodes_[i]->query( out);
            }

            // cross-sibling overlap checks
            for (int i = 0; i < 4; ++i) {
                for (int j = i + 1; j < 4; ++j) {
                    checkSiblingCollision(*nodes_[i], *nodes_[j], out);
                }
            }
        }
    }

    void QuadTree::checkSiblingCollision(const QuadTree &a, const QuadTree &b,
                                     std::vector<CollidersPair> &out) const
    {
        // compare all objects in node A against all objects in node B
        for (auto &objA : a.objects_) {
            for (auto &objB : b.objects_) {
                if (objA.box.intersects(objB.box)) {
                    out.emplace_back(objA.idx, objB.idx);
                }
            }
        }

        // recurse into children
        if (a.nodes_[0] && b.nodes_[0]) {
            for (int i = 0; i < 4; ++i)
                for (int j = 0; j < 4; ++j)
                    checkSiblingCollision(*a.nodes_[i], *b.nodes_[j], out);
        } else if (a.nodes_[0]) {
            // a has children, b doesn't — check a's subtree against b
            for (int i = 0; i < 4; ++i)
                checkSiblingCollision(*a.nodes_[i], b, out);
        } else if (b.nodes_[0]) {
            // b has children, a doesn't — check a against b's subtree
            for (int i = 0; i < 4; ++i)
                checkSiblingCollision(a, *b.nodes_[i], out);
        }
    }

    void QuadTree::objectQuery(QTEntry entry, std::vector<CollidersPair> &out) const {
        for (auto &obj: objects_) {
            if (entry.box.intersects(obj.box)) {
                out.emplace_back(entry.idx, obj.idx);
            }
        }

        if (nodes_[0]) {
            for (int i = 0; i < 4; ++i) {
                if (nodes_[i]->bounds_.intersects(entry.box)) {
                    nodes_[i]->objectQuery(entry, out);
                }
            }
        }
    }

    [[nodiscard]] int QuadTree::getChildIndex(const AABB &box) const {
        float midX = (bounds_.lower_bound.x + bounds_.upper_bound.x) * 0.5f;
        float midY = (bounds_.lower_bound.y + bounds_.upper_bound.y) * 0.5f;

        bool top = (box.upper_bound.y < midY);
        bool bottom = (box.lower_bound.y > midY);
        bool left = (box.upper_bound.x < midX);
        bool right = (box.lower_bound.x > midX);

        // NE = 0, NW = 1, SW = 2, SE = 3
        if (top && right) return 0;
        if (top && left) return 1;
        if (bottom && left) return 2;
        if (bottom && right) return 3;

        return -1; // does not fit fully inside a child
    }

    void QuadTree::subdivide() {
        float midX = (bounds_.lower_bound.x + bounds_.upper_bound.x) * 0.5f;
        float midY = (bounds_.lower_bound.y + bounds_.upper_bound.y) * 0.5f;

        nodes_[0] = std::make_unique<QuadTree>(level_ + 1,
                                               AABB{
                                                   {midX, bounds_.lower_bound.y}, {bounds_.upper_bound.x, midY}
                                               });
        nodes_[1] = std::make_unique<QuadTree>(level_ + 1,
                                               AABB{
                                                   {bounds_.lower_bound.x, bounds_.lower_bound.y}, {midX, midY}
                                               });
        nodes_[2] = std::make_unique<QuadTree>(level_ + 1,
                                               AABB{
                                                   {bounds_.lower_bound.x, midY}, {midX, bounds_.upper_bound.y}
                                               });
        nodes_[3] = std::make_unique<QuadTree>(level_ + 1,
                                               AABB{
                                                   {midX, midY}, {bounds_.upper_bound.x, bounds_.upper_bound.y}
                                               });
    }
}
