#include "core.h"
#include "Window.h"
#include "GameManager.hpp"

// Window Properties
int Window::width;
int Window::height;
const char* Window::windowTitle = "Model Environment";

// Objects to render
// Cube* Window::cube;
Client* client;

// Added by me:
uint8_t buf[4];
PlayerManager* Window::playerManager;

// Camera Properties
Camera* Cam;

// Interaction Variables
bool LeftDown, RightDown;
int MouseX, MouseY;

// The shader program id
GLuint Window::shaderProgram;

// Constructors and desctructors
bool Window::initializeProgram(Client& c) {
    client = &c;
    memset(buf, 0, 4);

    // Create a shader program with a vertex shader and a fragment shader.
    shaderProgram = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

    // Check the shader program.
    if (!shaderProgram) {
        std::cerr << "Failed to initialize shader program" << std::endl;
        return false;
    }

    return true;
}

bool Window::initializeObjects() {
    // Create a cube
    // cube = new Cube();
    // cube = new Cube(glm::vec3(-1, 0, -2), glm::vec3(1, 1, 1));
    InputManager::setDefaultKeys();
    // playerManager = new PlayerManager();
    // playerManager->mover = new
    // Mover("../assets/male_basic_walk_30_frames_loop/scene.gltf");
    return true;
}

void Window::cleanUp() {
    // Deallocate the objects.
    // delete cube;

    // Delete the shader program.
    glDeleteProgram(shaderProgram);
}

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

    // set up the camera
    Cam = new Camera();
    Cam->SetAspect(float(width) / float(height));

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

    Cam->SetAspect(float(width) / float(height));
}

// update and draw functions
float deltaTime = 0.01;

float currentTime = 0.0;
float accumulator = 0.0;

float t = 0.0f;
void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    float newTime = glfwGetTime();
    float frameTime = newTime - currentTime;
    currentTime = newTime;

    t += frameTime;
    if (t > 0.025f) {
        Packet* pkt = new Packet();
        pkt->write_int((int)PacketType::PLAYER_INPUT);
        uint8_t* buf = new uint8_t[4];
        buf[0] = InputManager::isKeyPressed(GLFW_KEY_W);
        buf[1] = InputManager::isKeyPressed(GLFW_KEY_A);
        buf[2] = InputManager::isKeyPressed(GLFW_KEY_S);
        buf[3] = InputManager::isKeyPressed(GLFW_KEY_D);
        for (int i = 0; i < 4; i++) {
            pkt->write_byte(buf[i]);
        }
        client->send(pkt);
        delete[] buf;
        t = 0;
    }
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    for (auto it : GameManager::instance().players) {
        // FIXME make thread safe and call only once
        if (it.second->mover == nullptr) {
            it.second->mover = new Mover(
                "../assets/male_basic_walk_30_frames_loop/scene.gltf");
        }
        it.second->mover->Update(deltaTime);
        it.second->mover->Draw(Cam->GetViewProjectMtx(), shaderProgram);
    }
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
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
    // NOTE: this should really be part of Camera::Update()
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