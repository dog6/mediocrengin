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


        bool CheckBoxBox(Collider& box_a, Collider& box_b) { 

            bool result = box_a.GetWorldBounds().Overlaps(box_b.GetWorldBounds());
            box_a.SetOverlapping(result);
            box_b.SetOverlapping(result);
            return result;
        }

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