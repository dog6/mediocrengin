#pragma once

#include <AVGNG/IJsonSerializable.hpp>

namespace ng::Core {

	class GameObject;

		class IComponent : public ng::Assets::IJsonSerializable {

		public:
			ng::Core::GameObject* owner = nullptr;
			virtual ~IComponent() = default;

			virtual void Start() {}
			virtual void Update(float deltaTime) {}
			
			/// <summary>
			/// Called to draw ImGui in inspector editor UI element
			/// </summary>
			virtual void OnInspectorGUI() = 0;

		};

}