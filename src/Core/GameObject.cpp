#include "AVGNG/Core/GameObject.hpp"

using namespace ng::Graphics;

namespace ng::Core {

    GameObject::GameObject(const char* name)
        : name(name), isActive(true)
    {
    }
    GameObject::~GameObject()
    {
        // unique_ptr auto cleanup components
    }



    IComponent* GameObject::AddComponentByName(const std::string& type) 
    {
        Debug::Log(LOG, "AddComponent '%s' to '%p'", type.c_str(), this);
        if (type == "Transform") return AddComponent<Transform>();
        if (type == "MeshRenderer") return AddComponent <MeshRenderer> ();
        if (type == "PhysicsBody") return AddComponent<PhysicsBody>();
        Debug::Log(LOG, "Failed to add component type '%s'", type.c_str());
        return nullptr;
    }

    // Can probably be simplified with T type
    IComponent* GameObject::GetComponentByName(const std::string& type)
    {
        if (type == "Transform") {
            auto* comp = GetComponent<Transform>();
            Debug::Log(LOG, "GetComponent Transform for '%s': %p", name.c_str(), comp);
            return comp;
        }
        if (type == "MeshRenderer") {
            auto* comp = GetComponent<MeshRenderer>();
            Debug::Log(LOG, "GetComponent MeshRenderer for '%s': %p", name.c_str(), comp);
            return comp;
        }

        if (type == "PhysicsBody"){
            auto* comp = GetComponent<PhysicsBody>();
            Debug::Log(LOG, "GetComponent PhysicsBody for '%s': %p", name.c_str(), comp);
            return comp;
        }

        return nullptr;
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }

}