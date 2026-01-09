#include "Game.hpp"

using namespace ng::Core;
using namespace ng::Graphics;

GLFWwindow* gameWindow;

Renderer renderer;
Camera* camera = new ng::Graphics::Camera();
Shader* shader = nullptr;

GameObject cubeObj;

// Helper Methods
void InitializeGameWindow(int window_width, int window_height, const char* windowName) {

    // Setup Debug Logging
    Debug::Init("G:/Projects/NG/AvgNGin/game.log");

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

Shader* LoadDefaultShader() {
    std::string fragShaderFolder = "G:/Projects/NG/AvgNGin/res/shaders/fragShaders/";
    std::string vertShaderFolder = "G:/Projects/NG/AvgNGin/res/shaders/vertexShaders/";

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

// Constructor
Game::Game() {
    this->windowTitle = "AvgNGin | v0.0.0";
    this->windowSize = glm::uvec2(1280, 720);
}

// Destructor
Game::~Game() {}

// Game Methods
void Game::Load()
{

    Debug::Log(LogLevel::LOG, "Loading game..");


    InitializeGameWindow(this->windowSize.x, this->windowSize.y, this->windowTitle.c_str());


    // Create gameObject
    cubeObj = GameObject();

    // Load Cube Mesh
    ng::Assets::ObjFileParser parser = ng::Assets::ObjFileParser();

    Shader* defaultShader = LoadDefaultShader();
    cubeObj.mesh = parser.LoadObjFromFile("C:/Users/dog/Desktop/grass_cube.obj");
    //cubeObj.mesh = parser.LoadObjFromFile("C:/Users/dog/Desktop/bronze-pickaxe-staging.obj");
    //cubeObj.mesh = parser.LoadObjFromFile("C:/Users/dog/Desktop/monkey.obj");
    cubeObj.mesh->shader = defaultShader;

    Debug::Log(LogLevel::LOG, "Loading completed.");

}

void Game::Start() {

    Debug::Log(LogLevel::LOG, "Game started.");


}

void Game::Run()
{
    Debug::Log(LogLevel::LOG, "Starting game loop.");


    float rotation = 0.0f;
    int frameCount = 0;  // Add counter

    while (!glfwWindowShouldClose(gameWindow))
    {
        rotation -= 0.01f;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_CULL_FACE);  // Before drawing

        cubeObj.SetPosition(glm::vec3(0, 0, -5));
        cubeObj.SetRotation(glm::vec3(rotation, 0, rotation));
      

        // Draw meshes
        cubeObj.mesh->Draw(camera, &cubeObj.transform);

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
