//
// Created by hugze on 17.11.2025.
//

#ifndef QUADTREE_DEBUG_DRAW_H
#define QUADTREE_DEBUG_DRAW_H
#include <SDL3/SDL_pixels.h>
#include "maths/vec2.h"


namespace physics { class QuadTree; }

namespace debug {
    void DrawQuadTree(const physics::QuadTree& tree, int depth = 0);
}



#endif //QUADTREE_DEBUG_DRAW_H
