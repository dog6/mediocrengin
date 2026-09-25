#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Debug.hpp" 
#include <typeinfo>

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
        Debug::Log(LOG, "AddComponent '%s' to '%p'", type.c_str(), (void*)this);

        if (type == "Transform")    return AddComponent<Transform>();
        if (type == "MeshRenderer") return AddComponent<MeshRenderer>();
        if (type == "PhysicsBody")  return AddComponent<PhysicsBody>();
        if (type == "Collider")     return AddComponent<Collider>();

        Debug::Log(LOG, "Failed to add component type '%s'", type.c_str());
        return nullptr;
    }

    IComponent* GameObject::GetComponentByName(const std::string& type)
    {
        IComponent* comp = nullptr;

        if      (type == "Transform")    comp = GetComponent<Transform>();
        else if (type == "MeshRenderer") comp = GetComponent<MeshRenderer>();
        else if (type == "PhysicsBody")  comp = GetComponent<PhysicsBody>();
        else if (type == "Collider")     comp = GetComponent<Collider>();
        else Debug::Log(LOG, "GetComponentByName: unknown component type '%s'", type.c_str());

        if (!comp) {
            Debug::Log(LOG, "GetComponent '%s' failed on object %p. Components on this object:",
                       type.c_str(), (void*)this);
            for (auto& c : components)
                Debug::Log(LOG, "  - %s", typeid(*c).name());
        }

        return comp;
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }

}