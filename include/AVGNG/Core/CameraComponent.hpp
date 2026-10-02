#pragma once

#include <nlohmann/json.hpp>
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Core/Transform.hpp"
#include "AVGNG/Graphics/Camera.hpp"

namespace ng::Core {

	// Drives the main camera from the Transform of its owner.
	// Put this component on a child GameObject of the player.
	// The camera follows the world position and the world rotation of that GameObject.
	class CameraComponent : public IComponent {

		Transform* tf = nullptr;

		// The camera that the renderer uses. One pointer for the whole engine.
		static inline ng::Graphics::Camera* s_mainCamera = nullptr;

	public:
		CameraComponent();
		~CameraComponent();

		// Call this function one time at startup.
		// Give it the same Camera object that the renderer uses.
		static void SetMainCamera(ng::Graphics::Camera* camera) { s_mainCamera = camera; }

		// Copies the world position and the look direction of the Transform to the camera.
		void Apply();

		// Inherited methods
		void Update(float deltaTime);
		void OnInspectorGUI() override;

		void Save(nlohmann::json& j) override;
		void Load(const nlohmann::json& j) override;
	};

}