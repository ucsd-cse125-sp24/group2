#include "core.h"
#include "Window.h"
#include "GameManager.hpp"
#include "components/RendererComponent.hpp"
#include "HUD.h"


// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";
ParticleSystem* Window::particleSystem;
PlayerEffects* Window::playerEffects;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;
SkyBox* Window::skybox;
// Constructors and desctructors
bool Window::initializeProgram() {
    // Create a shader program with a vertex shader and a fragment shader.
    GLuint res;
    res = Shader::LoadShader(ShaderType::STANDARD, "shaders/model.vert",
                             "shaders/model.frag");
    res = Shader::LoadShader(ShaderType::ANIMATED, "shaders/shader.vert",
                             "shaders/shader.frag");
    res = Shader::LoadShader(ShaderType::HUD, "shaders/HUD.vert",
                             "shaders/HUD.frag");
    res = Shader::LoadShader(ShaderType::SKYBOX, "shaders/skybox.vert",
                             "shaders/skybox.frag");
    res = Shader::LoadShader(ShaderType::PARTICLE, "shaders/particle.vert",
                             "shaders/particle.frag");

    // Check the shader program.
    if (!res) {
        std::cerr << "Failed to initialize one or more shaders." << std::endl;
        return false;
    }

    skybox = new SkyBox();

    // particleSystem = new ParticleSystem(glm::vec3(-10.0f, 1.0f, 0.0f), glm::vec3(1.0f, 50.0f, 300.0f), glm::vec3(800.0f), glm::vec4(254/255.0f, 212/255.0f, 123/255.0f, 1.0f), glm::vec4(254/255.0f, 109/255.0f, 41/255.0f, 1.0f), 20.0f, 30.0f, 50, 2.0f);
    playerEffects = new PlayerEffects();
    return true;
}

void Window::cleanUp() { Shader::CleanUp(); }

// for the Window
GLFWwindow* Window::createWindow(int width, int height) {
    // Initialize GLFW.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return NULL;
    }

    // 4x antialiasing.
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create the GLFW window.
    GLFWwindow* window =
        glfwCreateWindow(width, height, windowTitle, NULL, NULL);

    // Check if the window could not be created.
    if (!window) {
        std::cerr << "Failed to open GLFW window." << std::endl;
        return NULL;
    }

    // Make the context of the window.
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewInit();

    // Set swap interval to 1.
    glfwSwapInterval(0);

    // initialize the interaction variables
    LeftDown = RightDown = false;
    MouseX = MouseY = 0;

    // Call the resize callback to make sure things get drawn immediately.
    Window::resizeCallback(window, width, height);

    return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height) {
    Window::width = width;
    Window::height = height;
    // Set the viewport size.
    glViewport(0, 0, width, height);

    // Cam->SetAspect(float(width) / float(height));
}
bool start = false;
void Window::Render(GLFWwindow* window, Scene* scene, Camera* camera,
                    float deltaTime) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    skybox->draw(camera->GetViewMtx(), camera->GetProjMtx());
    // Render all objects in the scene
    for (auto& entity : scene->entities) {

        if (auto model = entity->GetComponent<Model>()) {
            NetTransform* transform = entity->GetComponent<NetTransform>();
            model->update(deltaTime);
        }
        if (auto animationPlayer = entity->GetComponent<AnimationPlayer>()) {
            animationPlayer->update(deltaTime);
        }
        if (auto huds = entity->GetComponent<HUDs>()) {
            huds->update(deltaTime);
        }

        if (auto renderer = entity->GetComponent<RendererComponent>())
            renderer->Render(camera->GetViewProjectMtx());
    }
    
    if(InputManager::IsKeyPressed(GLFW_KEY_SPACE)) {
        // std::cout<<"HERERERERE"<<std::endl;
        start = true;
       
    }
    if(start) {
        // playerEffects->playAttack(glm::vec3(-10.0f, 1.0f, 0.0f), glm::vec3(1.0f, 50.0f, 300.0f), deltaTime, 20.0f);
        // playerEffects->playHeal(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime, 20.0f);
        playerEffects->playSpeedUp(glm::vec3(0.0f, 1.0f, 0.0f), deltaTime, 20.0f);
        //  particleSystem->start(deltaTime);
    }
        playerEffects->update(deltaTime);
        playerEffects->draw(camera->GetViewMtx(), camera->GetProjMtx());


        // particleSystem->update(deltaTime);
        // particleSystem->draw(camera->GetViewMtx(), camera->GetProjMtx());
    // }

    // skybox->draw(camera->GetViewMtx(), camera->GetProjMtx());


    // Render 2D screen
    glDisable(GL_DEPTH_TEST);
    for (auto& entity : scene->entities) {
        if (auto huds = entity->GetComponent<HUDs>()) {
            huds->draw(camera->GetAspect());
        }
    }
    glEnable(GL_DEPTH_TEST);

    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

void Window::mouse_callback(GLFWwindow* window, int button, int action,
                            int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        LeftDown = (action == GLFW_PRESS);
    }
    if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        RightDown = (action == GLFW_PRESS);
    }
}