#include "AVGNG/Core/collision/CollisionSystem.hpp"
#include "AVGNG/Core/collision/Collider.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Graphics/DebugDraw.hpp"

namespace ng::Core {

    // bool CollisionSystem::CheckSphereSphere(Collider& a, Collider& b)

    void CollisionSystem::HandleCollision(Collider& a, Collider& b){
            a.SetOverlapping(true);
            a.SetOtherCollider(&b);
            b.SetOverlapping(true);
            b.SetOtherCollider(&a);
    }

    void CollisionSystem::Step(const std::vector<GameObject *> &objects)
    {
        // 1. Collect the active colliders and update their AABBs
        colliders.clear();

        for (GameObject *obj : objects)
        {
            if (!obj->isActive)
                continue;

            Collider *col = obj->GetComponent<Collider>();
            Transform *tf = obj->GetComponent<Transform>();
            if (!col || !tf || !col->IsActive())
                continue;

            col->SetOverlapping(false); // reset every frame, before the pair check
            col->SetOtherCollider(nullptr);
            col->UpdateBounds(tf->GetPosition(), tf->GetScale());
            colliders.push_back(col);
        }

        // 2. Broad phase, then narrow phase
        for (size_t i = 0; i < colliders.size(); ++i)
        {
            for (size_t j = i + 1; j < colliders.size(); ++j)
            {
                Collider &a = *colliders[i];
                Collider &b = *colliders[j];

                if (!a.GetWorldBounds().Overlaps(b.GetWorldBounds()))
                    continue;
                    
                    HandleCollision(a, b);


                // TODO: narrow phase goes here (shape pair test)
            }
        }
}



void CollisionSystem::DrawDebug(ng::Graphics::Camera& cam)
{
    if (!showGizmos) return;

    for (Collider* c : colliders) {
        if (!c->IsGizmoVisible()) continue;
        ng::Graphics::DebugDraw::Box(cam, c->GetWorldBounds(), c->GetGizmoColor());
    }
}

bool CollisionSystem::CheckCollision(Collider &a, Collider &b)
{
    switch (a.GetShapeType()) {

        case ShapeType::Box:
            switch (b.GetShapeType()) {
                default:
                case ShapeType::Box:
                    return CheckBoxBox(a, b);

                    // TODO: Check other shapes against other shapes
                    // for example:
                    // box vs sphere
                    // sphere vs sphere
                    // and so on for each primitive 

                // case ShapeType::Sphere:
                //     // return CheckBoxSphere(a, b);
            }

        break;

        // case ShapeType::Sphere:
        // {
        //     switch (b.GetShapeType()) {
        //         case ShapeType::Box:
        //             return CheckBoxSphere(b, a);

        //         case ShapeType::Sphere:
        //             return CheckSphereSphere(a, b);
        //     }

        //     break;
        // }

    }
    return false;
}

}