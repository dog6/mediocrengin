#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/Debug.hpp" 
#include <typeinfo>

using namespace ng::Graphics;

namespace ng::Core {

    namespace {

        // One row for each component type that Lua and the scene loader can use by name.
        struct ComponentType {
            const char* name;
            IComponent* (*add)(GameObject&);
            IComponent* (*get)(GameObject&);
        };

        template<typename T>
        ComponentType MakeType(const char* name)
        {
            return {
                name,
                [](GameObject& go) -> IComponent* { return go.AddComponent<T>(); },
                [](GameObject& go) -> IComponent* { return go.GetComponent<T>(); }
            };
        }

        // To add a new component type, add one line to this table.
        const ComponentType* FindType(const std::string& name)
        {
            static const ComponentType types[] = {
                MakeType<Transform>("Transform"),
                MakeType<MeshRenderer>("MeshRenderer"),
                MakeType<PhysicsBody>("PhysicsBody"),
                MakeType<Collider>("Collider"),
                MakeType<CameraComponent>("CameraComponent"),
            };

            for (const ComponentType& t : types) {
                if (name == t.name) return &t;
            }
            return nullptr;
        }

    }

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

        const ComponentType* t = FindType(type);
        if (t == nullptr) {
            Debug::Log(LOG, "Failed to add component type '%s'", type.c_str());
            return nullptr;
        }

        return t->add(*this);
    }

    IComponent* GameObject::GetComponentByName(const std::string& type)
    {
        const ComponentType* t = FindType(type);
        if (t == nullptr) {
            Debug::Log(LOG, "GetComponentByName: unknown component type '%s'", type.c_str());
            return nullptr;
        }

        IComponent* comp = t->get(*this);

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
        // Use an index. A component can add a component during Update.
        // That change can move the vector, and an iterator would be invalid.
        for (size_t i = 0; i < components.size(); ++i) {
            components[i]->Update(deltaTime);
        }
    }

}