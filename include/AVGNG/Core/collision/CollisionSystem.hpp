#pragma once

#include <vector>
#include "AVGNG/Core/collision/Collider.hpp"

namespace ng::Graphics { class Camera; }

namespace ng::Core {

    class GameObject;
    class Collider;

    class CollisionSystem {

        std::vector<Collider*> colliders; // rebuilt each frame
        bool showGizmos = true;

        // Ideas:
        // Create a PhysicsSystem class that handles physics "events"
        // A PhysicsEvent should be a data structure that stores information regarding two colliders impacting.
        // This information should include:
        // GameObject, Collider, Transform, and PhysicsBody references


        bool CheckBoxBox(Collider& box_a, Collider& box_b);
        bool CheckBoxSphere(Collider& box, Collider& sphere);
        bool CheckSphereSphere(Collider& sphere_a, Collider& sphere_b);


        void HandleCollision(Collider& a, Collider& b);

    public:

        // void Init();

        // Call one time each frame, after all objects moved.
        void Step(const std::vector<GameObject*>& objects);

        // Draws the AABB of each collider that has its gizmo visible.
        void DrawDebug(ng::Graphics::Camera& cam);

        void SetShowGizmos(bool v) { showGizmos = v; }
        bool GetShowGizmos() const { return showGizmos; }
        bool CheckCollision(Collider& a, Collider& b);


    };

}