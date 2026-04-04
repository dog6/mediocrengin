#pragma once

#include "AVGNG/Graphics/MeshRenderer.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include <iostream>

#include <memory>
#include <vector>
#include <type_traits>

namespace ng::Core {


	class GameObject {

    private:
        std::vector<std::unique_ptr<ng::Core::IComponent>> components;


	public:
        GameObject(const char* name);
        ~GameObject();

        bool isActive = true;
        std::string name = "";

        GameObject(const GameObject&) = delete;
        GameObject& operator=(const GameObject&) = delete;
        IComponent* AddComponentByName(const std::string& type);
        IComponent* GetComponentByName(const std::string& type);

        std::vector<IComponent*> GetAttachedComponents() {
            std::vector<IComponent*> result;
            for (auto& comp : components)
            {
                result.push_back(comp.get());  // get raw pointer, no ownership transfer
            }
            return result;
        }

        template<typename T, typename... Args> T*
            AddComponent(Args&&... args) {
            static_assert(std::is_base_of<IComponent, T>::value, "T must inherit from IComponent");

            auto comp = std::make_unique<T>(std::forward<Args>(args)...);
            comp->owner = this; // set owner

            T* ptr = comp.get(); // raw pointer to return
            components.push_back(std::move(comp));
            return ptr;
        }


        template<typename T> T*
            GetComponent()
        {

            for (auto& comp : components) {
                if (T* casted = dynamic_cast<T*>(comp.get())) {
                    return casted;
                }
            }
            return nullptr;

        }
        // Update all components
        void Update(float deltaTime);


	};

}
