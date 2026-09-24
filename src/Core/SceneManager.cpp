#include "AVGNG/Core/SceneManager.hpp"
#include "AVGNG/Core/Scene.hpp"

namespace ng::Core {

    std::vector<Scene*> SceneManager::scenes;
    Scene* SceneManager::activeScene = nullptr;


    void SceneManager::CreateNewScene(ng::Graphics::Camera* camera, glm::uvec2& viewportSize, const char* sceneName){
        activeScene = new Scene(camera, viewportSize, sceneName);
    }

    void SceneManager::SetActiveScene(Scene* scene)
    {
        if (scene == nullptr || scene == activeScene) return;

        delete activeScene;
        activeScene = scene;
        activeScene->Start();
    }

    Scene* SceneManager::GetActiveScene() { return activeScene; }

    int SceneManager::LoadActiveScene()
    {

        // Return -1 if activeScene == nullptr
        if (activeScene == nullptr) {
            Debug::Log(LogLevel::ERROR, "Failed to LoadActiveScene(), activeScene == nullptr");
            return -1;
        }

        // activeScene->Load(activeScene );

        return 0;
    }

    // Unloads active scene WITHOUT saving
    void SceneManager::Unload()
    {
        delete activeScene;
        activeScene = nullptr;
    }
}
