#include "AVGNG/Core/collision/Collider.hpp"
#include "AVGNG/Core/collision/SphereShape.hpp"
#include "AVGNG/Core/collision/BoxShape.hpp"
#include "AVGNG/Assets/JsonUtils.hpp"
#include <imgui/imgui.h>

#include <cfloat>

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

// The three values are half extents. A box with (0.5, 0.5, 0.5) is 1 unit wide.
void Collider::SetBoxShape(float hx, float hy, float hz)
{
    auto b = std::make_unique<BoxShape>();
    b->SetHalfExtents(glm::vec3(hx, hy, hz));
    colliderShape = std::move(b);
    shapeType = ShapeType::Box;
}

// "worldMatrix" must be Transform::GetWorldMatrix(). The local matrix
// gives a wrong box for a child object.
void Collider::UpdateBounds(const glm::mat4& worldMatrix)
{
    if (!colliderShape)
        return;

    // Transform the collider's local offset into world space.
    glm::vec3 worldCenter =
        glm::vec3(worldMatrix * glm::vec4(positionOffset, 1.0f));

    // Extract world scale from the transform matrix.
    glm::vec3 worldScale{
        glm::length(glm::vec3(worldMatrix[0])),
        glm::length(glm::vec3(worldMatrix[1])),
        glm::length(glm::vec3(worldMatrix[2]))
    };

    worldScale *= colliderScale;

    worldBounds = colliderShape->ComputeAABB(
        worldCenter,
        worldScale
    );
}

void Collider::OnInspectorGUI()
{
    // Give this component its own ID scope.
    // This stops label conflicts with other components (for example "Scale").
    ImGui::PushID(this);

    ImGui::Text("Collider Component [%p]", (void*)this);
    ImGui::Checkbox("Active", &isActive);

    // Shape selection
    enum ShapeChoice { ChoiceNone = 0, ChoiceSphere, ChoiceBox };
    const char* names[] = { "None", "Sphere", "Box" };

    int current = ChoiceNone;
    if (colliderShape)
        current = (colliderShape->Type() == ShapeType::Sphere) ? ChoiceSphere : ChoiceBox;

    if (ImGui::Combo("Shape", &current, names, IM_ARRAYSIZE(names))) {
        switch (current) {
            case ChoiceNone:   colliderShape.reset(); break;
            case ChoiceSphere: SetSphereShape();      break;
            case ChoiceBox:    SetBoxShape();         break;
        }
    }

    // Transform values
    ImGui::DragFloat3("Offset", &positionOffset.x, 0.01f, 0.0f, 0.0f, "%.3f");

    // The minimum value of 0.001 prevents a scale of 0.
    ImGui::DragFloat3("Scale", &colliderScale.x, 0.01f, 0.001f, FLT_MAX, "%.3f");

    // Gizmo settings
    ImGui::Checkbox("Show Gizmo", &gizmoVisible);
    ImGui::BeginDisabled(!gizmoVisible);
    ImGui::ColorEdit3("Gizmo Color", &gizmoColor.x);
    ImGui::EndDisabled();

    // Shape settings
    ImGui::Separator();
    if (colliderShape) {
        colliderShape->OnInspectorGUI();
    }
    else {
        ImGui::TextDisabled("No shape. This collider cannot collide.");
    }

    // Read-only collision data for debugging
    if (ImGui::TreeNode("Debug")) {
        ImGui::Text("Overlapping: %s", IsOverlapping() ? "yes" : "no");
        ImGui::Text("Other collider: %p", (void*)GetOtherCollider());

        const glm::vec3& n = GetContactNormal();
        ImGui::Text("Contact normal: %.2f, %.2f, %.2f", n.x, n.y, n.z);
        ImGui::Text("Penetration: %.4f", GetPenetration());

        ImGui::Text("Bounds min: %.2f, %.2f, %.2f",
                    worldBounds.min.x, worldBounds.min.y, worldBounds.min.z);
        ImGui::Text("Bounds max: %.2f, %.2f, %.2f",
                    worldBounds.max.x, worldBounds.max.y, worldBounds.max.z);

        ImGui::TreePop();
    }

    ImGui::PopID();
}

void Collider::Save(nlohmann::json& j)
{
    nlohmann::json& c = j["collider"];
    c["active"]       = isActive;
    c["gizmoVisible"] = gizmoVisible;
    c["gizmoColor"]   = ng::Assets::Vec3ToJson(gizmoColor);
    c["offset"]       = ng::Assets::Vec3ToJson(positionOffset);
    c["scale"]        = ng::Assets::Vec3ToJson(colliderScale);

    if (colliderShape) {
        nlohmann::json s;
        s["type"] = colliderShape->TypeName();
        colliderShape->Save(s);
        c["shape"] = s;
    }
}

void Collider::Load(const nlohmann::json& j)
{
    if (!j.contains("collider")) return;
    const nlohmann::json& c = j.at("collider");

    isActive     = c.value("active", isActive);
    gizmoVisible = c.value("gizmoVisible", gizmoVisible);

    gizmoColor     = ng::Assets::ReadVec3(c, "gizmoColor", gizmoColor);
    positionOffset = ng::Assets::ReadVec3(c, "offset", positionOffset);
    colliderScale  = ng::Assets::ReadVec3(c, "scale", colliderScale);

    if (c.contains("shape")) {
        const auto& s = c["shape"];
        colliderShape = CreateShape(s.value("type", ""));
        if (colliderShape) {
            colliderShape->Load(s);

            // Keep "shapeType" the same as the real shape.
            // The Set...Shape functions do this, but this path did not.
            shapeType = colliderShape->Type();
        }
    }
}