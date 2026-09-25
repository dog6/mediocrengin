#include "AVGNG/Core/Game.hpp"
#define NG_DEBUG_MODE
#define NG_DEVELOPER_MODE
using namespace ng::Core;
using namespace ng::Graphics;
using namespace ng::Assets;

#ifdef NG_DEVELOPER_MODE
using namespace ng::Editor;
#include "AVGNG/Editor/EditorUI.hpp"
#endif

#include "AVGNG/Scripting/LuaManager.hpp"

namespace ng {

    GLFWwindow* Game::gameWindow = nullptr;
    Camera* Game::camera = new ng::Graphics::Camera();
    SceneManager* Game::sceneManager = new SceneManager();
    Game* Game::Instance;

    // Helper methods
    void SetupImGUI(GLFWwindow* window) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        ImGui::StyleColorsDark(); // theme

        float main_scale = ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
        ImGuiStyle& style = ImGui::GetStyle();
        style.ScaleAllSizes(main_scale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
        style.FontScaleDpi = main_scale;
    }

    // Helper Methods
    static void InitializeGameWindow(int window_width, int window_height, const char* windowName) {

        Debug::Log(LOG, "Initializing game window..");

        if (!glfwInit()) {
            Debug::Log(FATAL, "Failed to initialize GLFW instance");
            return;
        }

        // Set OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create a windowed mode window and its OpenGL context
        Game::gameWindow = glfwCreateWindow(window_width, window_height, windowName, NULL, NULL);

        if (!Game::gameWindow) {
            Debug::Log(FATAL, "Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(Game::gameWindow);


        // Initialize GLAD
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
            Debug::Log(FATAL, "Failed to initialize GLAD");
            return;
        }

        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, window_width, window_height);

        Debug::Log(LOG, "Successfully created game window.");

    }

    // Constructor
    Game::Game() {
        this->windowTitle = "AvgNGin | v0.0.0";
        this->defaultWindowSize = glm::uvec2(1280, 720);
		this->currentWindowSize = this->defaultWindowSize;
        Game::Instance = this;
    }
    Game::Game(const char* title, glm::uvec2 size) {
        this->windowTitle = title;
        this->defaultWindowSize = size;
        this->currentWindowSize = this->defaultWindowSize;
        Game::Instance = this;
    }

    // Destructor
    Game::~Game() {
        this->sceneManager->Unload();
    }

    // Before Load
    void Game::Init()
    {
        Debug::Log(LogLevel::LOG, "Loading game..");
        InitializeGameWindow(this->defaultWindowSize.x, this->defaultWindowSize.y, this->windowTitle);
        this->viewportSize = this->defaultWindowSize;

        sceneManager->CreateNewScene(camera, this->viewportSize, "Development Scene");
        Time::Init();

        // Setup IMGUI
        SetupImGUI(gameWindow);

        // Initialize input handlers
        MouseInput::Init(*gameWindow);
        KeyboardInput::Init(gameWindow);
        Cursor::Init(gameWindow);

        // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
        ImGui_ImplOpenGL3_Init("#version 330");
        ImGui_ImplGlfw_InitForOpenGL(gameWindow, true);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    }

    // Game Methods
    void Game::Load()
    {

        // Load lua scene script
        ng::Scripting::LuaManager::Load("D:/Projects/CPP/smallengine/res/scripts/phys_dev_scene.lua");
        // ng::Scripting::LuaManager::Load("D:/Projects/CPP/smallengine/res/scripts/scene.lua");
        ng::Scripting::LuaManager::Load("D:/Projects/CPP/smallengine/res/scripts/noclip.lua");


        // We should have a way of specifying scene load order, perhaps by storing in a json file

        // TODO: Load a scene from disk here once a scene file is chosen
        // (see SceneJsonSerializer::DeserializeSceneFromJson). The scene is
        // currently populated by res/scripts/scene.lua.

        Debug::Log(LogLevel::LOG, "Loading completed.");

    }

    void Game::Start() {

        Debug::Log(LogLevel::LOG, "Game started.");
        this->sceneManager->LoadActiveScene();

        Scene* activeScene = sceneManager->GetActiveScene();
        if (activeScene != nullptr) activeScene->Start();

#ifdef NG_DEVELOPER_MODE
        EditorUI::ShowAllElements();
#endif

    }
   
    ImVec4 clearcolor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void Game::Run()
    {
        Debug::Log(LogLevel::LOG, "Starting game loop.");

        float cubeRotation = 0.0f;
        int frameCount = 0;  // Add counter

        while (!glfwWindowShouldClose(gameWindow))
        {
            Time::Update();
            glfwPollEvents();

            // Update everything in scene
            Scene* activeScene = sceneManager->GetActiveScene();
            activeScene->Update();
            viewportSize = glm::uvec2(currentWindowSize.x, currentWindowSize.y);
            glfwGetFramebufferSize(gameWindow, &currentWindowSize.x, &currentWindowSize.y);
            glViewport(0, 0, currentWindowSize.x, currentWindowSize.y);
            glClearColor(0.3f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Render to game window
            activeScene->Render();

            // Start ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

#ifdef NG_DEVELOPER_MODE            
            ng::Editor::EditorUI::Update();
#endif

            // End ImGui frame and render
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (frameCount++ == 60) {
                GLenum err = glGetError();
                if (err != GL_NO_ERROR) {
                    Debug::Log(LogLevel::ERROR, "OpenGL Error: %d", err);
                }
                frameCount = 0;
            }

            glfwSwapBuffers(gameWindow);
        }

        this->Exit();
    }

    void Game::Exit()
    {

        Debug::Log(LogLevel::LOG, "Exiting application.");
        Debug::Shutdown();
        glfwTerminate();

    }

    glm::uvec2 Core::Game::GetWindowSize()
    {
        if (Instance == nullptr) return glm::uvec2();
        return glm::uvec2(Instance->currentWindowSize.x, Instance->currentWindowSize.y);
    }

}