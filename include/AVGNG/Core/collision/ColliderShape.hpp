#pragma once

#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "AVGNG/Core/collision/AABB.hpp"

namespace ng::Core {

    enum class ShapeType { Sphere, Box, Count };

    class ColliderShape {

    public:
        virtual ~ColliderShape() = default;

        virtual ShapeType Type() const = 0;

        /// Name used in scene files: "sphere", "box"
        virtual const char* TypeName() const = 0;

        /// Returns the world-space AABB of this shape.
        /// center and scale are already in world space.
        virtual AABB ComputeAABB(const glm::vec3& center, const glm::vec3& scale) const = 0;

        virtual void Save(nlohmann::json& j) const = 0;
        virtual void Load(const nlohmann::json& j) = 0;

        /// Called to draw ImGui in inspector editor UI element
        virtual void OnInspectorGUI() = 0;
    };

}