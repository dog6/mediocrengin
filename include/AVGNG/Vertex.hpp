#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct Vertex {
    glm::vec3 position;  // Where the point is in 3D space (x, y, z)
    glm::vec3 normal;    // Which direction it's facing (for lighting)
    glm::vec2 texCoord;  // Where on a texture image this point maps to
};