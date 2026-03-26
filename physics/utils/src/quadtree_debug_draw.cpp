//
// Created by hugze on 17.11.2025.
//
#include "quadtree_debug_draw.h"

#include "shapes.h"
#include "engine/renderer.h"
#include "quadtree.h"

namespace debug {
    void DrawQuadTree(const physics::QuadTree &tree, int depth) {
        if (const auto *renderer = common::GetRenderer(); !renderer) return;

        common::DrawAABB(tree.bounds().center(), {tree.bounds().width(), tree.bounds().height()}, {1.f, 1.f, 1.f, 1.f}, false);

        for (auto &child: tree.children()) {
            if (child)
                DrawQuadTree(*child, depth + 1);
        }
    }
}
