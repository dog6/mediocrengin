#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "AVGNG/Graphics/Camera.hpp"

namespace ng::Core {

    class Scene;

    class SceneManager {

        private:
            static std::vector<ng::Core::Scene*> scenes;
            static Scene* activeScene;

        public:
            static void CreateNewScene(ng::Graphics::Camera* camera, glm::uvec2& viewportSize, const char* sceneName);
            static void SetActiveScene(Scene* scene); // deletes the current active scene and starts the new one
            static Scene* GetActiveScene();
            static int LoadActiveScene();
            static void Unload(); // unloads currently active scene
    };

}