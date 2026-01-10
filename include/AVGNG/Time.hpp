#pragma once

#include <GLFW/glfw3.h>

namespace ng::Core {

    class Time {
    private:
        static float deltaTime;
        static float fixedDeltaTime;
        static float lastFrameTime;
        static float timeScale;
        static float fps;
        static float fps_accum;

    public:
        static void Init();
        static void Update();

        // Getters
        static float DeltaTime() { return deltaTime * timeScale; }
        static float UnscaledDeltaTime() { return deltaTime; }
        static float GetTime() { return (float)glfwGetTime(); }
        static float GetTimeScale() { return timeScale; }
        static float FPS() { return fps; }

        // Setters
        static void SetTimeScale(float scale) { timeScale = scale; }
    };

}