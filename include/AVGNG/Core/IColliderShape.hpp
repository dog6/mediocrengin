#pragma once

namespace ng::Core {

    class IColliderShape {

    public:
        virtual ~IColliderShape() = default;

        /// <summary>
        /// Called to draw ImGui in inspector editor UI element
        /// </summary>
        virtual void OnInspectorGUI() = 0;
        

        // virtual bool HasCollision() = 0; // returns true if a collision has been detected
        // virtual glm::vec3 CollisionPoint() = 0; // Point of collision
        // virtual IColliderShape* GetColliderShape() = 0;


    };

}