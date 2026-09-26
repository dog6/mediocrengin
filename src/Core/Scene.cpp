#include "AVGNG/Core/Scene.hpp"

using namespace ng::Graphics;

namespace ng::Core {

	float deltaTime;



	Scene::Scene(Camera* _mainCamera, glm::uvec2& viewportSize, const char* _sceneName) : 
		sceneName(_sceneName), 
		mainCamera(_mainCamera), 
		isActive(true),
		viewportSize(viewportSize) { 
		this->skybox = new Skybox(viewportSize);
	}

	// Fallback viewport for scenes created without one; a reference member
	// must never bind to a temporary (it would dangle immediately).
	static glm::uvec2 s_defaultViewportSize(800, 600);

    Scene::Scene() :
		sceneName(""),
		mainCamera(nullptr),
		isActive(true),
		viewportSize(s_defaultViewportSize) {
		this->skybox = new Skybox(viewportSize);
    }

    Scene::~Scene()
    {
        // Clean up game objects
		for (auto* obj : gameObjectsInScene) {
			delete obj;
		}
		gameObjectsInScene.clear();

		delete skybox;
		skybox = nullptr;
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
	
	void Scene::Load(const std::string& filepath) {

		std::string file_content = ng::Assets::FileReader::ReadFile(filepath);
		if (file_content == "") {
			Debug::Log(ERROR, "Failed loading scene %s", filepath.c_str());
			return;
		}
		Debug::Log(LOG, "Loading scene: %s", sceneName.c_str());
	}

	void Scene::Start() {
		Debug::Log(LOG, "Starting scene: %s", sceneName.c_str());
		isActive = true;
		if (skybox != nullptr)
			this->skybox->Init();
	}

	void Scene::Update() {
		if (!isActive) return;

		deltaTime = Time::DeltaTime();

		// Update all game objects
		for (auto* obj : gameObjectsInScene) {
			if (obj->isActive) {
				// Call Update on components if you have that system
				obj->Update(deltaTime);
			}
		}

		// Collision runs after movement
		collisionSystem.Step(gameObjectsInScene);

		// Update Lua VM
		ng::Scripting::LuaManager::Update(deltaTime);

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

		collisionSystem.DrawDebug(*mainCamera);
		
		if (skybox != nullptr)
			this->skybox->Render(*mainCamera);
	}

	void Scene::Unload() {
		Debug::Log(LOG, "Unloading scene: %s", sceneName.c_str());
		isActive = false;
	}

	Camera* Scene::GetActiveCamera() const {
		return mainCamera;
	}

}