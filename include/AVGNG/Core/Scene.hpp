#pragma once

#include <vector>
#include <string>
#include <memory>

#include "AVGNG/Core/GameObject.hpp"
#include "AVGNG/Core/IComponent.hpp"
#include "AVGNG/Scripting/LuaManager.hpp"
#include "AVGNG/Graphics/MeshRenderer.hpp"
#include "AVGNG/Graphics/Camera.hpp"
#include "AVGNG/Graphics/Skybox.hpp"
#include "AVGNG/Core/Time.hpp"

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
		Scene();
		~Scene();

		std::string sceneName;
		bool isActive;

		GameObject* CreateGameObject(const std::string& name = "GameObject");
		void AddGameObject(GameObject* obj);
		void RemoveGameObject(GameObject* obj);
		GameObject* FindGameObjectByName(const std::string& name);

		void Load(const std::string& filepath);
		void Start();
		void Update();
		void Render();
		void Unload();

		void SetActiveCamera(ng::Graphics::Camera* cam);
		ng::Graphics::Camera* GetActiveCamera() const;

		const std::string GetName() const { return sceneName; }
		const std::vector<GameObject*>& GetGameObjects() const { return gameObjectsInScene; }

		bool IsActive() const { return isActive; }
		void SetActive(bool active) { isActive = active; }

	};

}