#include "AVGNG/Scripting/PhysicsRegistrar.hpp"

#include <sol/sol.hpp>
#include <glm/glm.hpp>
#include <tuple>

#include "AVGNG/Core/physics/PhysicsBody.hpp"
#include "AVGNG/Core/collision/Collider.hpp"

using namespace ng::Core;

namespace ng::Scripting {

void PhysicsRegistrar::RegisterCollider(sol::state& lua) {
    lua.new_usertype<Collider>("Collider",
        sol::base_classes, sol::bases<IComponent>(),

        // Lua cannot see C++ default arguments, so use lambdas.
        "SetSphere", [](Collider& c, sol::optional<float> r) {
            c.SetSphereShape(r.value_or(0.5f));
        },
        "SetBox", [](Collider& c, float x, float y, float z) {
            c.SetBoxShape(x, y, z);
        },

        "SetPositionOffset", [](Collider& c, float x, float y, float z) {
            c.SetPositionOffset(glm::vec3(x, y, z));
        },
        "SetColliderScale", [](Collider& c, float x, float y, float z) {
            c.SetColliderScale(glm::vec3(x, y, z));
        },

        "HasCollision", &Collider::IsOverlapping,
        "SetActive", &Collider::SetActive,
        "IsActive", &Collider::IsActive,

        "SetGizmoVisible", &Collider::SetGizmoVisible,
        "IsGizmoVisible", &Collider::IsGizmoVisible,
        "SetGizmoColor", [](Collider& c, float r, float g, float b) {
            c.SetGizmoColor(glm::vec3(r, g, b));
        },
        "GetGizmoColor", [](Collider& c) {
            glm::vec3 col = c.GetGizmoColor();
            return std::make_tuple(col.x, col.y, col.z);
        }
    );
}

void PhysicsRegistrar::RegisterPhysicsBody(sol::state& lua) {
    lua.new_usertype<PhysicsBody>("PhysicsBody",
        sol::base_classes, sol::bases<IComponent>(),

        "SetLinearAcceleration", [](PhysicsBody& self, float x, float y, float z) { self.SetLinearAcceleration(glm::vec3(x, y, z)); },
        "SetLinearVelocity", [](PhysicsBody& self, float x, float y, float z) { self.SetLinearVelocity(glm::vec3(x, y, z)); },
        "SetAngularAcceleration", [](PhysicsBody& self, float x, float y, float z) { self.SetAngularAcceleration(glm::vec3(x, y, z)); },
        "SetAngularVelocity", [](PhysicsBody& self, float x, float y, float z) { self.SetAngularVelocity(glm::vec3(x, y, z)); },

        "GetLinearAcceleration", [](PhysicsBody& self) {
            glm::vec3 v = self.GetLinearAcceleration();
            return std::make_tuple(v.x, v.y, v.z);
        },
        "GetLinearVelocity", [](PhysicsBody& self) {
            glm::vec3 v = self.GetLinearVelocity();
            return std::make_tuple(v.x, v.y, v.z);
        },
        "GetAngularVelocity", [](PhysicsBody& self) {
            glm::vec3 v = self.GetAngularVelocity();
            return std::make_tuple(v.x, v.y, v.z);
        },
        "GetAngularAcceleration", [](PhysicsBody& self) {
            glm::vec3 v = self.GetAngularAcceleration();
            return std::make_tuple(v.x, v.y, v.z);
        }
    );
}

void PhysicsRegistrar::Register(sol::state& lua) {
    RegisterPhysicsBody(lua);
    RegisterCollider(lua);
}

} // namespace ng::Scripting