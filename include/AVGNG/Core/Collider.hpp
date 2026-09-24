#pragma once

#include "AVGNG/Core/IColliderShape.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include <glm/glm.hpp>

namespace ng::Core {

    class Collider {

            glm::vec3 positionOffset;
            glm::vec3 colliderScale;
            IColliderShape* colliderShape;
            bool isActive; // can this collider be collided with?

        public:

            void SetPositionOffset(glm::vec3 posOffset);
            void SetColliderScale(glm::vec3 colScale);
            void SetColliderShape(IColliderShape& shape);

            void SetActive(bool enabled) { isActive = enabled; }
            bool IsActive() { return isActive; }



    };

}