#include "AVGNG/Core/collision/Collider.hpp"
#include "AVGNG/Core/collision/SphereShape.hpp"
#include "AVGNG/Core/collision/BoxShape.hpp"
#include <imgui/imgui.h>

using namespace ng::Core;

namespace {

    // Makes a shape from its name in the scene file.
    std::unique_ptr<ColliderShape> CreateShape(const std::string& type)
    {
        if (type == "sphere") return std::make_unique<SphereShape>();
        if (type == "box")    return std::make_unique<BoxShape>();
        return nullptr; // unknown type
    }

}

void Collider::SetSphereShape(float radius)
{
    auto s = std::make_unique<SphereShape>();
    s->SetRadius(radius);
    colliderShape = std::move(s);
    shapeType = ShapeType::Sphere;
}

void Collider::SetBoxShape(float hx, float hy, float hz)
{
    auto b = std::make_unique<BoxShape>();
    b->SetHalfExtents(glm::vec3(hx, hy, hz));
    colliderShape = std::move(b);
    shapeType = ShapeType::Box;
}

void Collider::UpdateBounds(const glm::vec3& position, const glm::vec3& scale)
{
    if (!colliderShape) return;

    glm::vec3 center = position + positionOffset;
    worldBounds = colliderShape->ComputeAABB(center, scale * colliderScale);
}

void Collider::OnInspectorGUI()
{
    ImGui::Text("Collider Component [%p]", (void*)this);

    ImGui::Checkbox("Active", &isActive);
    ImGui::Checkbox("Show Gizmo", &gizmoVisible);
    ImGui::ColorEdit3("Gizmo Color", &gizmoColor.x);
    ImGui::DragFloat3("Offset", &positionOffset.x, 0.01f);
    ImGui::DragFloat3("Scale", &colliderScale.x, 0.01f);

    const char* names[] = { "None", "Sphere", "Box" };
    int current = 0;
    if (colliderShape)
        current = (colliderShape->Type() == ShapeType::Sphere) ? 1 : 2;

    if (ImGui::Combo("Shape", &current, names, 3)) {
        if (current == 0)      colliderShape.reset();
        else if (current == 1) SetSphereShape();
        else                   SetBoxShape();
    }

    if (colliderShape) colliderShape->OnInspectorGUI();
}

void Collider::Save(nlohmann::json& j)
{
    j["active"]       = isActive;
    j["gizmoVisible"] = gizmoVisible;
    j["gizmoColor"]   = { gizmoColor.x, gizmoColor.y, gizmoColor.z };
    j["offset"]       = { positionOffset.x, positionOffset.y, positionOffset.z };
    j["scale"]        = { colliderScale.x, colliderScale.y, colliderScale.z };

    if (colliderShape) {
        nlohmann::json s;
        s["type"] = colliderShape->TypeName();
        colliderShape->Save(s);
        j["shape"] = s;
    }
}

void Collider::Load(const nlohmann::json& j)
{
    isActive     = j.value("active", true);
    gizmoVisible = j.value("gizmoVisible", false);

    auto readVec3 = [&](const char* key, glm::vec3& out) {
        if (j.contains(key)) {
            const auto& a = j[key];
            out = glm::vec3(a[0].get<float>(), a[1].get<float>(), a[2].get<float>());
        }
    };
    readVec3("gizmoColor", gizmoColor);
    readVec3("offset", positionOffset);
    readVec3("scale", colliderScale);

    if (j.contains("shape")) {
        const auto& s = j["shape"];
        colliderShape = CreateShape(s.value("type", ""));
        if (colliderShape) colliderShape->Load(s);
    }
}