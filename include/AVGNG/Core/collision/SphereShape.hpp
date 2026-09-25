#pragma once

#include <algorithm>
#include "AVGNG/Core/collision/ColliderShape.hpp"
#include <imgui/imgui.h>

namespace ng::Core {

    class SphereShape : public ColliderShape {

        float radius = 0.5f;

    public:
        void SetRadius(float r) { radius = r; }
        float GetRadius() const { return radius; }

        ShapeType Type() const override { return ShapeType::Sphere; }
        const char* TypeName() const override { return "sphere"; }

        AABB ComputeAABB(const glm::vec3& center, const glm::vec3& scale) const override {
            // A sphere uses the largest scale axis.
            glm::vec3 s = glm::abs(scale);
            float m = std::max({s.x, s.y, s.z});
            glm::vec3 r(radius * m);
            return { center - r, center + r };
        }

        void Save(nlohmann::json& j) const override { j["radius"] = radius; }
        void Load(const nlohmann::json& j) override { radius = j.value("radius", 0.5f); }

        void OnInspectorGUI() override {
            ImGui::DragFloat("Radius", &radius, 0.01f, 0.001f, 1000.0f);
        }
    };

}