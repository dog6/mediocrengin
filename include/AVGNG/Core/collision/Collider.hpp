#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include "AVGNG/Core/collision/AABB.hpp"
#include "AVGNG/Core/collision/ColliderShape.hpp"
#include "AVGNG/Core/IComponent.hpp"

// Do not include GameObject.hpp here.
// GameObject.hpp includes this file, so it would make a circular include.

namespace ng::Core {

    class Collider : public IComponent {

        glm::vec3 positionOffset{0.0f};
        glm::vec3 colliderScale{1.0f};
        std::unique_ptr<ColliderShape> colliderShape;
        AABB worldBounds;
        bool gizmoVisible = false;
        glm::vec3 gizmoColor{0.0f, 1.0f, 0.0f}; // RGB, each value 0 to 1
        bool isActive = true; // can this collider be collided with?
        // Collider* overlappingCollider; // should probably be a list or something at some point
        ShapeType shapeType = ShapeType::Box;
        bool overlapping = false; // is this collider colliding with something?


    public:
        void SetPositionOffset(const glm::vec3& posOffset) { positionOffset = posOffset; }
        void SetColliderScale(const glm::vec3& colScale)   { colliderScale = colScale; }
        const glm::vec3& GetPositionOffset() const { return positionOffset; }
        const glm::vec3& GetColliderScale() const  { return colliderScale; }

        void SetShape(std::unique_ptr<ColliderShape> shape) { colliderShape = std::move(shape); }
        ColliderShape* GetShape() const { return colliderShape.get(); }

        void SetOverlapping(bool hasCol) { overlapping = hasCol; }
        bool IsOverlapping() { return overlapping; }

        // These two functions make a new shape. They also work from Lua.
        void SetSphereShape(float radius = 0.5f);
        void SetBoxShape(float hx = 0.5f, float hy = 0.5f, float hz = 0.5f);

        const AABB& GetWorldBounds() const { return worldBounds; }

        void SetActive(bool enabled) { isActive = enabled; }
        bool IsActive() const { return isActive; }

        void SetGizmoVisible(bool isVisible) { gizmoVisible = isVisible; }
        bool IsGizmoVisible() const { return gizmoVisible; }
        void SetGizmoColor(const glm::vec3& c) { gizmoColor = c; }
        const glm::vec3& GetGizmoColor() const { return gizmoColor; }

        ShapeType GetShapeType() { return shapeType; }
        // The collision system calls this one time each frame, before the broad phase.
        void UpdateBounds(const glm::vec3& position, const glm::vec3& scale);

        void OnInspectorGUI() override;
        void Save(nlohmann::json& j) override;
        void Load(const nlohmann::json& j) override;
    };

}