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
        if (type == "Transform") return GetComponent<Transform>();
        if (type == "MeshRenderer") return GetComponent <MeshRenderer>();
        return nullptr;
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }

}