#include "AVGNG/Core/SceneManager.hpp"
#include "AVGNG/Core/Scene.hpp"
#include "AVGNG/Core/Debug.hpp"

namespace ng::Core {

    std::vector<Scene*> SceneManager::scenes;
    Scene* SceneManager::activeScene  = nullptr;
    Scene* SceneManager::pendingScene = nullptr;

    Scene* SceneManager::CreateNewScene(ng::Graphics::Camera* camera,
                                        glm::uvec2& viewportSize,
                                        const char* sceneName)
    {
        if (FindScene(sceneName) != nullptr) {
            Debug::Log(LogLevel::WARN, "Scene '%s' already exists.", sceneName);
            return nullptr;
        }

        Scene* scene = new Scene(camera, viewportSize, sceneName);
        scenes.push_back(scene);

        // The first scene is the active scene
        if (activeScene == nullptr) activeScene = scene;

        return scene;
    }

    Scene* SceneManager::FindScene(const std::string& sceneName)
    {
        for (Scene* scene : scenes) {
            if (scene->GetName() == sceneName) return scene;
        }
        return nullptr;
    }

    Scene* SceneManager::GetActiveScene() { return activeScene; }

    bool SceneManager::RequestSceneChange(const std::string& sceneName)
    {
        Scene* scene = FindScene(sceneName);

        if (scene == nullptr) {
            Debug::Log(LogLevel::ERROR, "Scene '%s' does not exist.", sceneName.c_str());
            return false;
        }

        if (scene == activeScene) return false; // Already active

        pendingScene = scene;
        return true;
    }

    bool SceneManager::RequestSceneChange(Scene* scene)
    {
        if (scene == nullptr) {
            Debug::Log(LogLevel::ERROR, "Scene '%s' does not exist.", scene->GetName().c_str());
            return false;
        }

        if (scene == activeScene) return false; // Already active

        pendingScene = scene;
        return true;
    }

    void SceneManager::ApplyPendingSceneChange()
    {
        if (pendingScene == nullptr) return;

        Scene* next = pendingScene;
        pendingScene = nullptr;

        Debug::Log(LogLevel::LOG, "Changing scene to '%s'", next->GetName().c_str());

        // The old scene stays in the list. Do not delete it here.
        activeScene = next;
        activeScene->Start();
    }

    int SceneManager::LoadActiveScene()
    {
        if (activeScene == nullptr) {
            Debug::Log(LogLevel::ERROR, "Failed to LoadActiveScene(), activeScene == nullptr");
            return -1;
        }

        // Call the load function of the scene here
        // when Scene has one. Examples: shaders, skybox, meshes.
        return 0;
    }

    void SceneManager::StartActiveScene()
    {
        if (activeScene == nullptr) {
            Debug::Log(LogLevel::ERROR, "Failed to start scene, activeScene is nil");
            return;
        }

        activeScene->Start();
        Debug::Log(LogLevel::LOG, "Starting active scene '%s'", activeScene->GetName().c_str());
    }

    void SceneManager::Update()
    {
        if (activeScene != nullptr) activeScene->Update();
    }

    void SceneManager::Render()
    {
        if (activeScene != nullptr) activeScene->Render();
    }

    void SceneManager::Unload()
    {
        for (Scene* scene : scenes) delete scene;

        scenes.clear();
        activeScene  = nullptr;
        pendingScene = nullptr;
    }

} // namespace ng::Core