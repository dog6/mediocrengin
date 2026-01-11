#include <AVGNG/GameObject.hpp>

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



    Component* GameObject::AddComponentByName(const std::string& type) 
    {
        if (type == "Transform") return AddComponent<Transform>();
        if (type == "MeshRenderer") return AddComponent <MeshRenderer> ();
        return nullptr;
    }

    Component* GameObject::GetComponentByName(const std::string& type)
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
        return nullptr;
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }

}