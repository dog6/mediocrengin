#include <AVGNG/Game.hpp>

using namespace ng::Core;
using namespace ng::Graphics;
using namespace ng::Assets;

namespace ng {

GLFWwindow* gameWindow;


Camera* camera = new ng::Graphics::Camera();
Scene* activeScene = new Scene(camera, "Development Scene");

Shader* shader = nullptr;

//GameObject* cubeObj = new GameObject("Cube");
//GameObject* terrainObj = new GameObject("Terrain");

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
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

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
    void Core::Game::Init()
    {
        Debug::Log(LogLevel::LOG, "Loading game..");
        InitializeGameWindow(this->windowSize.x, this->windowSize.y, this->windowTitle);
        
        Time::Init();

        ng::Scripting::LuaManager::Init(activeScene);

    }

    // Game Methods
    void Game::Load()
    {
        // Load lua scene script
         ng::Scripting::LuaManager::Load(activeScene, "./res/scripts/dev.lua");

        // Load default shader
        Shader* defaultShader = ShaderLoader::LoadDefaultShader();

        // Load active scene
        if (activeScene != nullptr) {
            activeScene->Load();
        }

        activeScene->Load();

        // Load Cube Mesh
        //cubeObj = LoadObjAsGameObject("Cube", "./res/models/mdl_grass_cube.obj", defaultShader);
        //cubeObj = LoadObjAsGameObject("Cube", "./res/models/textured_cube.obj", defaultShader);
        // Load Terrain Mesh
        //terrainObj = LoadObjAsGameObject("Terrain", "res/models/mdl_terrain.obj", defaultShader);


        Debug::Log(LogLevel::LOG, "Loading completed.");

    }

    void Game::Start() {

        Debug::Log(LogLevel::LOG, "Game started.");
        activeScene->Start();

    }

    void Game::Run()
    {
        Debug::Log(LogLevel::LOG, "Starting game loop.");

        float cubeRotation = 0.0f;
        int frameCount = 0;  // Add counter

        while (!glfwWindowShouldClose(gameWindow))
        {
            Time::Update();

            // Update
            //cubeRotation -= 2.0f * Time::DeltaTime(); // 2 rads/second
            //cubeTF->SetRotation(glm::vec3(cubeRotation, 0, cubeRotation));
            activeScene->Update();

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_CULL_FACE);  // Before drawing

            // Draw    
            /*cubeMR->Draw(*camera, *cubeTF);
            terrainMR->Draw(*camera, *terrainTF);*/
            activeScene->Render();

            // Check for GL errors 
            if (frameCount++ == 60) {

                // Check for openGL errors
                GLenum err = glGetError();
                if (err != GL_NO_ERROR) {
                    Debug::Log(LogLevel::ERROR, "OpenGL Error: %d", err);
                }
            }

            glfwSwapBuffers(gameWindow);
            glfwPollEvents();
        }

        this->Exit();
    }

    void Game::Exit()
    {

        Debug::Log(LogLevel::LOG, "Exiting application.");

        glfwTerminate();

    }

}