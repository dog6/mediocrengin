#pragma once

#include <glm/glm.hpp>

namespace ng::Assets {

    class Math {

        
        public:
            static glm::vec3 Vec3Abs(glm::vec3 v) {
                return glm::vec3(glm::abs(v.x), glm::abs(v.y), glm::abs(v.z));
            }

    };

}