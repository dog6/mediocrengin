#pragma once

#include <vector>
#include <string>
#include <memory>

#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Core/LuaManager.hpp"
#include "AVGNG/Renderer/MeshRenderer.hpp"
#include "AVGNG/Renderer/Camera.hpp"
#include "AVGNG/Renderer/Skybox.hpp"

namespace ng::Core {

	class Scene {

	private:
		ng::Graphics::Camera* mainCamera;
		std::vector<std::unique_ptr<IComponent>> components;
		std::vector<GameObject*> gameObjectsInScene;
		glm::uvec2& viewportSize;
		ng::Graphics::Skybox* skybox;

	public:
		Scene(ng::Graphics::Camera* _mainCamera, glm::uvec2& viewportSize, const char* _sceneName = "New Scene");
		~Scene();

		const char* sceneName;
		bool isActive;

		GameObject* CreateGameObject(const std::string& name = "GameObject");
		void AddGameObject(GameObject* obj);
		void RemoveGameObject(GameObject* obj);
		GameObject* FindGameObjectByName(const std::string& name);

		void Load();
		void Start();
		void Update();
		void Render();
		void Unload();

		void SetActiveCamera(ng::Graphics::Camera* cam);
		ng::Graphics::Camera* GetActiveCamera() const;

		const std::string GetName() const { return std::string(sceneName); }
		const std::vector<GameObject*>& GetGameObjects() const { return gameObjectsInScene; }

		bool IsActive() const { return isActive; }
		void SetActive(bool active) { isActive = active; }

	};

}