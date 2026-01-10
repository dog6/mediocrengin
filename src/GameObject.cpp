#include <AVGNG/GameObject.hpp>

namespace ng::Core {

  
    GameObject::GameObject(const char* name)
        : name(name), isActive(true)
    {
    }

    GameObject::~GameObject()
    {
        // unique_ptr auto cleanup components
    }

    void GameObject::Update(float deltaTime)
    {
        for (auto& comp : components) {
            comp->Update(deltaTime);
        }
    }

}