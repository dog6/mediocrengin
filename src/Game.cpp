#include <AVGNG/Game.hpp>

using namespace ng::Core;
using namespace ng::Graphics;
using namespace ng::Assets;

#ifdef NG_DEVELOPER_MODE
using namespace ng::Editor;
#endif
namespace ng {

    GLFWwindow* gameWindow;

    Camera* Game::camera = new ng::Graphics::Camera();
    Scene* Game::activeScene = new Scene(camera, "Development Scene");

    // Helper Methods
    static void InitializeGameWindow(int window_width, int window_height, const char* windowName) {

        // Setup Debug Logging
        Debug::Init("game.log");

        Debug::Log(LOG, "Initializing game window..");

        if (!glfwInit()) {
            Debug::Log(FATAL, "Failed to initialize GLFW instance");
            return;
        }

        // Set OpenGL version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        /* Create a windowed mode window and its OpenGL context */
        gameWindow = glfwCreateWindow(window_width, window_height, windowName, NULL, NULL);

        if (!gameWindow) {
            Debug::Log(FATAL, "Failed to create GLFW window");
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(gameWindow);


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
        this->windowSize = glm::uvec2(1280, 720);
    }
    Game::Game(const char* title, glm::uvec2 size) {
        this->windowTitle = title;
        this->windowSize = size;
    }
    
    // Destructor
    Game::~Game() {}

    // Before Load
    void Game::Init()
    {
        Debug::Log(LogLevel::LOG, "Loading game..");
        InitializeGameWindow(this->windowSize.x, this->windowSize.y, this->windowTitle);
        
        Time::Init();

        // Setup IMGUI
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
    }

    // Game Methods
    void Game::Load()
    {

        // Load lua scene script
         ng::Scripting::LuaManager::Load(activeScene, "./res/scripts/voxel_wg.lua");
		 ng::Scripting::LuaManager::Load(activeScene, "./res/scripts/noclip.lua");

        // Load active scene
        if (activeScene != nullptr) {
            activeScene->Load();
        }

        activeScene->Load();

        Debug::Log(LogLevel::LOG, "Loading completed.");

    }

    void Game::Start() {

        Debug::Log(LogLevel::LOG, "Game started.");
        activeScene->Start();

#ifdef NG_DEVELOPER_MODE
        EditorUI::ShowInspector();
        EditorUI::ShowHierarchy();
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

            int display_w, display_h;
            glfwGetFramebufferSize(gameWindow, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            activeScene->Update();
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

        glfwTerminate();

    }

}