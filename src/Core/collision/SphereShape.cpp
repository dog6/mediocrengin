#pragma once

#include "AVGNG/Core/collision/ColliderShape.hpp"

namespace ng::Core {

    class SphereShape : public ColliderShape {

        float radius{0.5f};

    public:
        void SetRadius(float r) { radius = r; }
        float GetRadius() const { return radius; }

        ShapeType Type() const override { return ShapeType::Sphere; }
        const char* TypeName() const override { return "sphere"; }

        AABB ComputeAABB(const glm::vec3& center, const glm::vec3& scale) const override {
            glm::vec3 absScale = glm::abs(scale);
            float maxScale = std::max({ absScale.x, absScale.y, absScale.z });
            float r = radius * maxScale;
            return { center - glm::vec3(r), center + glm::vec3(r) };
        }

        void Save(nlohmann::json& j) const override {
            j["radius"] = radius;
        }

        void Load(const nlohmann::json& j) override {
            if (j.contains("radius")) {
                radius = j["radius"].get<float>();
            }
        }

        void OnInspectorGUI() override;
    };

}