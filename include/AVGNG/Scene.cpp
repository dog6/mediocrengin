#include "Scene.hpp"

using namespace ng::Graphics;

namespace ng::Core {

	Scene::Scene(Camera* _mainCamera, const char* _sceneName)
	{
		this->sceneName = _sceneName;
		this->mainCamera = _mainCamera;
		Debug::Log(LOG, "Scene created: %s", sceneName);
	}

	Scene::~Scene() {
		// Clean up game objects
		for (auto* obj : gameObjectsInScene) {
			delete obj;
		}
		gameObjectsInScene.clear();
	}

	void Scene::SetActiveCamera(ng::Graphics::Camera* cam)
	{
		if (cam != nullptr) mainCamera = cam;
		else Debug::Log(LogLevel::ERROR, "mainCamera must not be null");
	}

	GameObject* Scene::CreateGameObject(const std::string& name)
	{
		GameObject* obj = new GameObject(name.c_str());
		obj->AddComponent<Transform>();
		gameObjectsInScene.push_back(obj);
		return obj;
	}

	void Scene::AddGameObject(GameObject* obj)
	{
		if (obj != nullptr) {
			gameObjectsInScene.push_back(obj);
		}
	}

	void Scene::RemoveGameObject(GameObject* obj) {
		auto it = std::find(gameObjectsInScene.begin(), gameObjectsInScene.end(), obj);
		if (it != gameObjectsInScene.end()) {
			gameObjectsInScene.erase(it);
		}
	}

	GameObject* Scene::FindGameObjectByName(const std::string& name) {
		for (auto* obj : gameObjectsInScene) {
			if (obj->name == name) {
				return obj;
			}
		}
		return nullptr;
	}
	
	void Scene::Load() {
		Debug::Log(LOG, "Loading scene: %s", sceneName);
		// Override in derived classes for custom loading
	}

	void Scene::Start() {
		Debug::Log(LOG, "Starting scene: %s", sceneName);
		isActive = true;
	}

	void Scene::Update() {
		if (!isActive) return;

		// Update all game objects
		for (auto* obj : gameObjectsInScene) {
			if (obj->isActive) {
				// Call Update on components if you have that system
				//obj->Update()
			}
		}
	}

	void Scene::Render() {
		if (!isActive || mainCamera == nullptr) return;

		// Render all objects with MeshRenderer
		for (auto* obj : gameObjectsInScene) {
			if (!obj->isActive) continue;

			auto* meshRenderer = obj->GetComponent<MeshRenderer>();
			auto* transform = obj->GetComponent<Transform>();

			if (meshRenderer != nullptr && transform != nullptr) {
				meshRenderer->Draw(*mainCamera, *transform);
			}
		}
	}

	void Scene::Unload() {
		Debug::Log(LOG, "Unloading scene: %s", sceneName);
		isActive = false;
	}

	Camera* Scene::GetActiveCamera() const {
		return mainCamera;
	}

}