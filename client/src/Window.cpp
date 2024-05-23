#include "core.h"
#include "Window.h"
#include "GameManager.hpp"
#include "components/RendererComponent.hpp"

// HealthBar* Window::healthBar;
// Metronome* Window::metronome;
// TeamInfo* Window::teamInfo;
HUDs* Window::hud;
ParticleSystem* Window::particleSystem;

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

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
    res = Shader::LoadShader(ShaderType::PARTICLE, "shaders/particle.vert",
                            "shaders/particle.frag");
    // Check the shader program.
    if (!res) {
        std::cerr << "Failed to initialize one or more shaders." << std::endl;
        return false;
    }
    hud = new HUDs();
    //particleSystem = new ParticleSystem(glm::vec3(0.0f), glm::vec4(254/255.0f, 212/255.0f, 123/255.0f, 1.0f), glm::vec4(254/255.0f, 109/255.0f, 41/255.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2.0f, 1000, 10.0f);
    particleSystem = new ParticleSystem(glm::vec3(0.0f), glm::vec3(0.0f), 3.0f, 1000, 0.5f, "particleStar.png", "../assets");
    // healthBar = new HealthBar(glm::vec3(-0.6f, 0.95f, 0.0f), 0.7f);
    // healthBar = new HealthBar(glm::vec3(-0.60f, 0.98f, 0.0f), 0.45f, 0.4f);
    // metronome = new Metronome(60.0f);
    // teamInfo = new TeamInfo(3);
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

void Window::Render(GLFWwindow* window, Scene* scene, Camera* camera,
                    float deltaTime) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render all objects in the scene
    for (auto& entity : scene->entities) {
        if (auto model = entity->GetComponent<Model>())
            model->update(deltaTime, entity->position);

        if (auto animationPlayer = entity->GetComponent<AnimationPlayer>())
            animationPlayer->update(deltaTime);

        if (auto renderer = entity->GetComponent<RendererComponent>())
            renderer->Render(camera->GetViewProjectMtx());
    }
    if(InputManager::isKeyPressed(GLFW_KEY_SPACE)) {
        // for(int i = 0; i < 1000; i++) {
            // float radius = 0.5f;
            particleSystem->spawnParticles(glm::vec3(0.0f, 0.0f, 0.0f));
        // } 
    }
    particleSystem->update(deltaTime);
    particleSystem->draw(camera->GetViewProjectMtx());

    hud->update(deltaTime);
    hud->draw(camera->GetAspect());
    // if(InputManager::isKeyPressed(GLFW_KEY_Q))
    //     healthBar->currHealth -= 0.05f; 
    // if(InputManager::isKeyPressed(GLFW_KEY_E))
    //     healthBar->currHealth += 0.05f;
    // metronome->update(deltaTime);
    // healthBar->update();
    // teamInfo->update();
 
    // healthBar->draw(camera->GetAspect());
    // metronome->draw(camera->GetAspect());
    // teamInfo->draw(camera->GetAspect());
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

void Window::cursor_callback(GLFWwindow* window, double currX, double currY) {
    int maxDelta = 100;
    int dx = glm::clamp((int)currX - MouseX, -maxDelta, maxDelta);
    int dy = glm::clamp(-((int)currY - MouseY), -maxDelta, maxDelta);

    MouseX = (int)currX;
    MouseY = (int)currY;

    // Move camera
    // TODO: this should really be part of Camera::Update()
    Camera* Cam = GameManager::instance().cam;
    if (LeftDown) {
        const float rate = 1.0f;
        Cam->SetAzimuth(Cam->GetAzimuth() + dx * rate);
        Cam->SetIncline(
            glm::clamp(Cam->GetIncline() - dy * rate, -90.0f, 90.0f));
    }
    if (RightDown) {
        const float rate = 0.005f;
        float dist =
            glm::clamp(Cam->GetDistance() * (1.0f - dx * rate), 0.01f, 1000.0f);
        Cam->SetDistance(dist);
    }
}