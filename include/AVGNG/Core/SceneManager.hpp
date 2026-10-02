#pragma once
#include <string>
#include <vector>
#include <glm/glm.hpp>

namespace ng::Graphics { class Camera; }

namespace ng::Core {

class Scene;

class SceneManager {
public:
    SceneManager() = delete; // All members are static

    // Creates a scene and keeps it in the list.
    // The first scene becomes the active scene.
    static Scene* CreateNewScene(ng::Graphics::Camera* camera,
                                 glm::uvec2& viewportSize,
                                 const char* sceneName);

    static Scene* FindScene(const std::string& sceneName);
    static Scene* GetActiveScene();

    // Saves a request. The change occurs in ApplyPendingSceneChange().
    static bool RequestSceneChange(const std::string& sceneName);
    static bool RequestSceneChange(Scene* sceneName);

    // Call this function one time at the start of each frame.
    static void ApplyPendingSceneChange();

    // Life cycle functions
    static int  LoadActiveScene();
    static void StartActiveScene();
    static void Update();
    static void Render();

    // Deletes all scenes WITHOUT saving
    static void Unload();

private:
    static std::vector<Scene*> scenes;
    static Scene* activeScene;
    static Scene* pendingScene;
};

} // namespace ng::Core