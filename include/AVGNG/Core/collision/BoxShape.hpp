#pragma once

#include "AVGNG/Core/collision/ColliderShape.hpp"

namespace ng::Core {

    class BoxShape : public ColliderShape {

        glm::vec3 halfExtents{0.5f};

    public:
        void SetHalfExtents(const glm::vec3& h) { halfExtents = h; }
        const glm::vec3& GetHalfExtents() const { return halfExtents; }

        ShapeType Type() const override { return ShapeType::Box; }
        const char* TypeName() const override { return "box"; }

        AABB ComputeAABB(const glm::vec3& center, const glm::vec3& scale) const override {
            glm::vec3 h = halfExtents * glm::abs(scale);
            return { center - h, center + h };
        }

        void Save(nlohmann::json& j) const override {
            j["halfExtents"] = { halfExtents.x, halfExtents.y, halfExtents.z };
        }

        void Load(const nlohmann::json& j) override {
            if (j.contains("halfExtents")) {
                const auto& h = j["halfExtents"];
                halfExtents = glm::vec3(h[0].get<float>(), h[1].get<float>(), h[2].get<float>());
            }
        }

        void OnInspectorGUI() override;
    };

}