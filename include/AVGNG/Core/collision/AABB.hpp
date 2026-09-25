#pragma once

#include <glm/glm.hpp>

namespace ng::Core {

    struct AABB {
        glm::vec3 min{0.0f};
        glm::vec3 max{0.0f};

        glm::vec3 Center() const { return (min + max) * 0.5f; }
        glm::vec3 Size() const   { return max - min; }

        bool Overlaps(const AABB& o) const {
            return (min.x <= o.max.x && max.x >= o.min.x) &&
                   (min.y <= o.max.y && max.y >= o.min.y) &&
                   (min.z <= o.max.z && max.z >= o.min.z);
        }
    };

}