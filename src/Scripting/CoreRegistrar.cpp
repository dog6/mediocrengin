#include "AVGNG/Scripting/CoreRegistrar.hpp"

#include "AVGNG/Core/Game.hpp"
#include "AVGNG/Core/Scene.hpp"
#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Graphics/MeshRenderer.hpp"
#include "AVGNG/Core/physics/PhysicsBody.hpp"
#include "AVGNG/Core/collision/Collider.hpp"

using namespace ng::Core;
using namespace ng::Graphics;

namespace ng::Scripting {

    static void RegisterGame(sol::state& lua) {
        lua.new_usertype<Game>("Game",
            "GetActiveScene", [](Game& game) {
                return SceneManager::GetActiveScene();
            }
        );
    }
	
    static void RegisterScene(sol::state& lua) {
        lua.new_usertype<Scene>("Scene",
            "CreateGameObject", &Scene::CreateGameObject,
            sol::meta_function::equal_to, [](const Scene& a, const Scene& b) { return &a == &b; },
            sol::meta_function::less_than, [](const Scene& a, const Scene& b) { return &a < &b; },

            "FindGameObject", &Scene::FindGameObjectByName
        );
    }
		
    static void RegisterTransform(sol::state& lua) {
        lua.new_usertype<Transform>("Transform",
            sol::base_classes, sol::bases<IComponent>(),
            "SetPosition", [](Transform& self, float x, float y, float z) {
                self.SetPosition(glm::vec3(x, y, z));
            },
            "SetRotation", [](Transform& self, float x, float y, float z) {
                self.SetRotation(glm::vec3(x, y, z));
            },
            "SetScale", [](Transform& self, float x, float y, float z) {
                self.SetScale(glm::vec3(x, y, z));
            },
            "GetPosition", [](Transform& self) { return self.GetPosition(); },
            "GetRotation", [](Transform& self) { return self.GetRotation(); },
            "GetScale", [](Transform& self) { return self.GetScale(); },
            "Parent", [](Transform& self) { return self.GetParent(); },
            "SetParent", [](Transform& self, Transform* parent) { self.SetParent(parent); },
            "AddChild", [](Transform& self, Transform* child) { self.AddChild(child); },
            "RemoveChild", [](Transform& self, Transform* child) { self.RemoveChild(child); },
            "GetChildCount", [](Transform& self) { self.GetChildCount(); },
            "ClearParent", [](Transform& self) { self.ClearParent(); }
        );
    }
		
    static sol::object WrapComponent(sol::this_state s, const std::string& name, IComponent* comp)
    {
        if (!comp) return sol::nil;
        if (name == "Transform")    return sol::make_object(s, static_cast<Transform*>(comp));
        if (name == "MeshRenderer") return sol::make_object(s, static_cast<MeshRenderer*>(comp));
        // Note: static_cast targets below require their headers included above if active
        if (name == "PhysicsBody")  return sol::make_object(s, static_cast<PhysicsBody*>(comp));
        if (name == "Collider")     return sol::make_object(s, static_cast<Collider*>(comp));
        return sol::make_object(s, comp);
    }

    static void RegisterGameObject(sol::state& lua)
    {
        lua.new_usertype<GameObject>("GameObject",
            "name", &GameObject::name,
            "AddComponent", [](GameObject& self, const std::string& name, sol::this_state s) {
                return WrapComponent(s, name, self.AddComponentByName(name));
            },
            "GetComponent", [](GameObject& self, const std::string& name, sol::this_state s) {
                return WrapComponent(s, name, self.GetComponentByName(name));
            }
        );
    }

    void CoreRegistrar::Register(sol::state& lua) {
        RegisterGame(lua);
        RegisterScene(lua);
        RegisterTransform(lua);
        RegisterGameObject(lua);
    }

}