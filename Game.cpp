#include "Game.hpp"


using namespace ng;
using namespace ng::Core;
using namespace ng::Graphics;

GLFWwindow* gameWindow;

Renderer renderer;
Camera* camera = new ng::Graphics::Camera();
Shader* shader = nullptr;

GameObject cubeObj;

// Helper Methods
void InitializeGameWindow(int window_width, int window_height, const char* windowName) {

    printf("Initializing game window..\n");

    if (!glfwInit()) {
        printf("Failed to initialize GLFW instance\n");
        return;
    }

    // Set OpenGL version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    gameWindow = glfwCreateWindow(window_width, window_height, windowName, NULL, NULL);

    if (!gameWindow) {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(gameWindow);


    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return;
    }

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window_width, window_height);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    printf("Successfully created game window\n");

}

void LoadDefaultShader() {
    std::string fragShaderFolder = "G:/Projects/NG/AvgNGin/res/shaders/fragShaders/";
    std::string vertShaderFolder = "G:/Projects/NG/AvgNGin/res/shaders/vertexShaders/";

    std::string vertShaderPath = vertShaderFolder + "defaultShader.vert";
    std::string fragShaderPath = fragShaderFolder + "defaultShader.frag";

    printf("Loading default vertex shader: %s\n", vertShaderPath.c_str());
    printf("Loading default fragment shader: %s\n", fragShaderPath.c_str());

    shader = ng::Graphics::Shader::LoadShader(vertShaderPath.c_str(), fragShaderPath.c_str());
    
    if (shader != nullptr) {
        printf("Successfully loaded shader ID: %d\n", shader->ID);
    }

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
    printf("Loading game..\n");

    InitializeGameWindow(this->windowSize.x, this->windowSize.y, this->windowTitle.c_str());


    // Create gameObject
    cubeObj = GameObject();

    // Load Cube Mesh
    ng::Assets::ObjFileParser parser = ng::Assets::ObjFileParser();
    cubeObj.mesh = parser.LoadObjFromFile("C:/Users/dog/Desktop/monkey.obj");

    // Load Shaders
    LoadDefaultShader();

    printf("Loading completed!\n");

}

void Game::Start() {

    printf("Game started.\n");

}

void Game::Run()
{
    printf("Starting game loop..\n");

    float rotation = 0.0f;
    int frameCount = 0;  // Add counter

    while (!glfwWindowShouldClose(gameWindow))
    {
        rotation -= 0.01f;

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = camera->GetViewMatrix();
        glm::mat4 projection = camera->GetProjectionMatrix(1280, 720);
        glm::mat4 model = cubeObj.transform.GetModelMatrix();

        cubeObj.SetPosition(glm::vec3(0, 0, rotation));
        cubeObj.SetRotation(glm::vec3(0, 0, rotation));

        shader->Use();
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        shader->SetMat4("model", model);
        shader->SetVec3("baseColor", glm::vec3(.2, .4, 1.0));
        
        // Draw meshes
        cubeObj.mesh->Draw();

        // Log once every (300) frames, so @ 60fps, ~every 5 seconds
        if (frameCount++ == 300) {
            
            // Check for openGL errors
            GLenum err = glGetError();
            if (err != GL_NO_ERROR) {
                printf("OpenGL Error: %d\n", err);
            }



        }

        glfwSwapBuffers(gameWindow);
        glfwPollEvents();
    }

    this->Exit();
}

void Game::Exit()
{

    printf("Exiting application\n");

    glfwTerminate();

}
