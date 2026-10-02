#include "AVGNG/Core/collision/CollisionSystem.hpp"
#include "AVGNG/Core/collision/Collider.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Graphics/DebugDraw.hpp"
#include "AVGNG/Core/collision/SphereShape.hpp"
#include "AVGNG/Core/Debug.hpp"
namespace ng::Core {

    // bool CollisionSystem::CheckSphereSphere(Collider& a, Collider& b)

    bool CollisionSystem::CheckBoxBox(Collider &box_a, Collider &box_b)
{
    auto a = box_a.GetWorldBounds();
    auto b = box_b.GetWorldBounds();

    if (!a.Overlaps(b)) return false;

    glm::vec3 halfA = (a.max - a.min) * 0.5f;
    glm::vec3 halfB = (b.max - b.min) * 0.5f;
    glm::vec3 delta = b.Center() - a.Center();

    // Overlap depth on each axis
    glm::vec3 overlap = (halfA + halfB) - glm::abs(delta);

    // Use the axis with the smallest overlap
    int axis = 0;
    if (overlap.y < overlap[axis]) axis = 1;
    if (overlap.z < overlap[axis]) axis = 2;

    // Normal points from A to B
    glm::vec3 normal(0.0f);
    normal[axis] = (delta[axis] >= 0.0f) ? 1.0f : -1.0f;
    float penetration = overlap[axis];

    box_a.SetOverlapping(true);
    box_b.SetOverlapping(true);
    box_a.SetContact(normal, penetration);
    box_b.SetContact(-normal, penetration);
    return true;
}

bool CollisionSystem::CheckSphereSphere(Collider &sphere_a, Collider &sphere_b)
{
    auto boundsA = sphere_a.GetWorldBounds();
    auto boundsB = sphere_b.GetWorldBounds();

    glm::vec3 centerA = boundsA.Center();
    glm::vec3 centerB = boundsB.Center();

    // World radius, including scale
    float radA = (boundsA.max.x - boundsA.min.x) * 0.5f;
    float radB = (boundsB.max.x - boundsB.min.x) * 0.5f;

    glm::vec3 diff = centerB - centerA;
    float distSq = glm::dot(diff, diff);
    float radiusSum = radA + radB;

    if (distSq > radiusSum * radiusSum) return false;

    float dist = std::sqrt(distSq);

    // Normal points from A to B. Use a default if the centers are equal.
    glm::vec3 normal = (dist > 0.0001f) ? (diff / dist) : glm::vec3(0.0f, 1.0f, 0.0f);
    float penetration = radiusSum - dist;

    sphere_a.SetOverlapping(true);
    sphere_b.SetOverlapping(true);
    sphere_a.SetContact(normal, penetration);
    sphere_b.SetContact(-normal, penetration);
    return true;
}

bool CollisionSystem::CheckBoxSphere(Collider &box, Collider &sphere)
{
    auto boxBounds = box.GetWorldBounds();
    auto sphereBounds = sphere.GetWorldBounds();

    glm::vec3 center = sphereBounds.Center();
    float radius = (sphereBounds.max.x - sphereBounds.min.x) * 0.5f;

    // Closest point on the box to the sphere center
    glm::vec3 closest = glm::clamp(center, boxBounds.min, boxBounds.max);
    glm::vec3 diff = center - closest;
    float distSq = glm::dot(diff, diff);

    if (distSq > radius * radius) return false;

    glm::vec3 normal;
    float penetration;

    if (distSq > 0.000001f) {
        // The center is outside the box.
        float dist = std::sqrt(distSq);
        normal = diff / dist;
        penetration = radius - dist;
    }
    else {
        // The center is inside the box. Find the closest face.
        glm::vec3 toMin = center - boxBounds.min;
        glm::vec3 toMax = boxBounds.max - center;

        float best = toMin.x;
        normal = glm::vec3(-1.0f, 0.0f, 0.0f);

        if (toMax.x < best) { best = toMax.x; normal = glm::vec3( 1.0f, 0.0f, 0.0f); }
        if (toMin.y < best) { best = toMin.y; normal = glm::vec3(0.0f, -1.0f, 0.0f); }
        if (toMax.y < best) { best = toMax.y; normal = glm::vec3(0.0f,  1.0f, 0.0f); }
        if (toMin.z < best) { best = toMin.z; normal = glm::vec3(0.0f, 0.0f, -1.0f); }
        if (toMax.z < best) { best = toMax.z; normal = glm::vec3(0.0f, 0.0f,  1.0f); }

        penetration = radius + best;
    }

    // Normal points from the box to the sphere
    box.SetOverlapping(true);
    sphere.SetOverlapping(true);
    box.SetContact(normal, penetration);
    sphere.SetContact(-normal, penetration);
    return true;
}

   void CollisionSystem::HandleCollision(Collider &a, Collider &b)
    {
        ColliderShape* shapeA = a.GetShape();
        ColliderShape* shapeB = b.GetShape();
        if (!shapeA || !shapeB) return;
        
        ShapeType typeA = shapeA->Type();
        ShapeType typeB = shapeB->Type();

        bool hit = false;

        if (typeA == ShapeType::Box && typeB == ShapeType::Box) {
            hit = CheckBoxBox(a, b);
        }
        else if (typeA == ShapeType::Sphere && typeB == ShapeType::Sphere) {
            hit = CheckSphereSphere(a, b);
        }
        else if (typeA == ShapeType::Box && typeB == ShapeType::Sphere) {
            hit = CheckBoxSphere(a, b);
        }
        else if (typeA == ShapeType::Sphere && typeB == ShapeType::Box) {
            // CheckBoxSphere needs the box as the first argument.
            hit = CheckBoxSphere(b, a);
        }

        if (!hit) return;
        // Debug::Log(DEV, "Contact. Penetration: %f", penetration);
        // The check functions set the overlap flag and the contact.
        // Set the other-collider pointers here.
        a.SetOtherCollider(&b);
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
            col->UpdateBounds(tf->GetWorldMatrix());
            colliders.push_back(col);
        }

        // Actual collision pass
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
    if (!showGizmos)
        return;

    Debug::Log(VERBOSE, "Drawing %d collider gizmos..", colliders.size());

    for (Collider* c : colliders)
    {
        if (!c->IsGizmoVisible())
            continue;

        const AABB& bounds = c->GetWorldBounds();

        glm::vec3 center = (bounds.min + bounds.max) * 0.5f;
        glm::vec3 size   = bounds.max - bounds.min;

        glm::mat4 model =
            glm::translate(glm::mat4(1.0f), center) *
            glm::scale(glm::mat4(1.0f), size);

        ng::Graphics::DebugDraw::Box(
            cam,
            model,
            c->GetGizmoColor()
        );
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
                case ShapeType::Sphere:
                    return CheckBoxSphere(a,b);

                    // TODO: Check other shapes against other shapes
                    // for example:
                    // box vs sphere
                    // sphere vs sphere
                    // and so on for each primitive 

                // case ShapeType::Sphere:
                //     // return CheckBoxSphere(a, b);
            }

        break;

        case ShapeType::Sphere:
        {
            switch (b.GetShapeType()) {
                case ShapeType::Box:
                    return CheckBoxSphere(b, a);

                case ShapeType::Sphere:
                    return CheckSphereSphere(a, b);
            }

            break;
        }

    }
    return false;
}

}