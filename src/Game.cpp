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

static Shader* LoadDefaultShader() {
    std::string fragShaderFolder = "res/shaders/fragShaders/";
    std::string vertShaderFolder = "res/shaders/vertexShaders/";

    std::string vertShaderPath = vertShaderFolder + "defaultShader.vert";
    std::string fragShaderPath = fragShaderFolder + "defaultShader.frag";

    Debug::Log(DEBUG, "Loading default vertex shader: %s", vertShaderPath.c_str());
    Debug::Log(DEBUG, "Loading default fragment shader: %s\n", fragShaderPath.c_str());

    shader = ng::Graphics::Shader::LoadShader(vertShaderPath.c_str(), fragShaderPath.c_str());
    
    if (shader != nullptr) {
        Debug::Log(LOG, "Successfully loaded shader ID: %d", shader->ID);
        return shader;
    }
    return nullptr;
}

static Core::GameObject* LoadObjAsGameObject(const char* name, const char* objFilePath, Shader* shader) {

    if (!std::filesystem::exists(objFilePath)) {
        Debug::Log(ERROR, "Failed to load .obj with path '%s'.", objFilePath);
    }

    GameObject* result = new GameObject(name);

    result->AddComponent<ng::Graphics::MeshRenderer>();
    result->AddComponent<ng::Core::Transform>();

    MeshRenderer* resultMeshRenderer = result->GetComponent<ng::Graphics::MeshRenderer>();

    ObjFileParser parser = ObjFileParser();
    resultMeshRenderer->mesh = parser.LoadObjFromFile(objFilePath);
    
    if (shader != nullptr) {
        resultMeshRenderer->shader = shader;
    }
    else if (Shader* defaultShader = LoadDefaultShader()) {
        resultMeshRenderer->shader = defaultShader;

        if (defaultShader == nullptr) {
            Debug::Log(LogLevel::ERROR, "Failed to load shader while loading .obj as gameObject.\n.OBJ path: '%s'.", objFilePath);
        }
    }

    return result;

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

    // Game Methods
    void Game::Load()
    {

        Debug::Log(LogLevel::LOG, "Loading game..");
        InitializeGameWindow(this->windowSize.x, this->windowSize.y, this->windowTitle);

        Time::Init();

        // Load default shader
        Shader* defaultShader = LoadDefaultShader();


        // Load active scene
        if (activeScene != nullptr) {
            activeScene->Load();
        }

        // Load Cube Mesh
        //cubeObj = LoadObjAsGameObject("Cube", "./res/models/mdl_grass_cube.obj", defaultShader);
        //cubeObj = LoadObjAsGameObject("Cube", "./res/models/textured_cube.obj", defaultShader);
        // Load Terrain Mesh
        //terrainObj = LoadObjAsGameObject("Terrain", "res/models/mdl_terrain.obj", defaultShader);


        Debug::Log(LogLevel::LOG, "Loading completed.");

    }

    void Game::Start() {

        Debug::Log(LogLevel::LOG, "Game started.");


    }

    void Game::Run()
    {
        Debug::Log(LogLevel::LOG, "Starting game loop.");


        //// Setup cube components
        //Transform* cubeTF = cubeObj->GetComponent<Transform>();
        //MeshRenderer* cubeMR = cubeObj->GetComponent<MeshRenderer>();
        //cubeTF->SetPosition(glm::vec3(0,0,-10));

        //// Setup terrain components
        //Transform* terrainTF = terrainObj->GetComponent<Transform>();
        //MeshRenderer* terrainMR = terrainObj->GetComponent<MeshRenderer>();
        //terrainTF->SetPosition(glm::vec3(0, -3, -10));


        float cubeRotation = 0.0f;
        int frameCount = 0;  // Add counter

        while (!glfwWindowShouldClose(gameWindow))
        {
            Time::Update();

            // Update
            //cubeRotation -= 2.0f * Time::DeltaTime(); // 2 rads/second
            //cubeTF->SetRotation(glm::vec3(cubeRotation, 0, cubeRotation));

            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glDisable(GL_CULL_FACE);  // Before drawing

            // Draw    
            /*cubeMR->Draw(*camera, *cubeTF);
            terrainMR->Draw(*camera, *terrainTF);*/


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