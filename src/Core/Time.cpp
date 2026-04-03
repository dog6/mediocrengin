#include "AVGNG/Core/Time.hpp"

namespace ng::Core {

    float Time::deltaTime = 0.0f;
    float Time::fixedDeltaTime = 1.0f / 60.0f; // physics dt
    float Time::lastFrameTime = 0.0f;
    float Time::timeScale = 1.0f;
    float Time::fps = 0.0f;
    float Time::fps_accum = 0.0f;

    void Time::Init() {
        lastFrameTime = (float)glfwGetTime();
        deltaTime = 0.0f;
        fps = 0.0f;
        fps_accum = 0.0f;
    }

    void Time::Update() {
        float currentTime = (float)glfwGetTime();
        deltaTime = currentTime - lastFrameTime;
        lastFrameTime = currentTime;

        if (deltaTime > 0.25f) deltaTime = 0.25f;

        // Calculate FPS (update every 0.5 seconds)
        fps_accum += deltaTime;
        if (fps_accum >= 0.5f) {
            fps = 1.0f / deltaTime;
            fps_accum = 0.0f;
        }

    }

} 