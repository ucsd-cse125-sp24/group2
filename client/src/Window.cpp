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

void Window::idleCallback() {
    // Perform any updates as necessary.
    Cam->Update();

    // spins cube
    // cube->update();
    float newTime = glfwGetTime();
    float frameTime = newTime - currentTime;
    currentTime = newTime;
    // playerManager->mover->Update(frameTime);
    //  player->update(frameTime);
    //  accumulator += frameTime;

    // while (accumulator >= deltaTime) {
    //     mover->UpdatePhysics(deltaTime);
    //     accumulator -= deltaTime;
    // }
}

void Window::displayCallback(GLFWwindow* window) {
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Render the object.
    for (auto it : GameManager::instance().players) {
        // FIXME make thread safe and only once
        if (it.second->mover == nullptr) {
            it.second->mover = new Mover(
                "../assets/male_basic_walk_30_frames_loop/scene.gltf");
        }
        it.second->mover->Update(deltaTime);
        it.second->mover->Draw(Cam->GetViewProjectMtx(), shaderProgram);
    }
    // playerManager->mover->Draw(Cam->GetViewProjectMtx(), shaderProgram);
    //     // TODO draw object
    //     // mover->Draw(Cam->GetViewProjectMtx(), shaderProgram);
    //     for (auto kv : GameManager::instance().players) {
    //         // kv.second->mover->Draw(Cam->GetViewProjectMtx(),
    //         shaderProgram); std::cout << "Player " << kv.second->id << ": "
    //                   << glm::to_string(kv.second->mover->position) <<
    //                   std::endl;
    //     }

    //     // Gets events, including input such as keyboard and mouse or window
    //     // resizing.
    glfwPollEvents();
    // Swap buffers.
    glfwSwapBuffers(window);
}

// helper to reset the camera
void Window::resetCamera() {
    Cam->Reset();
    Cam->SetAspect(float(Window::width) / float(Window::height));
}

// callbacks - for Interaction
void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action,
                         int mods) {
    /*
     * TODO: Modify below to add your key callbacks.
     */

    // Check for a key press.
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            // Close the window. This causes the program to also terminate.
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;

        case GLFW_KEY_R:
            resetCamera();
            break;

        case GLFW_KEY_W:
            // Close the window. This causes the program to also terminate.
            std::cout << "Movin forward!" << std::endl;
            // mover->velocityHeading += glm::vec3(0,0,-1);
            buf[0] = 1;
            break;

        case GLFW_KEY_A:
            // Close the window. This causes the program to also terminate.
            std::cout << "Movin left!" << std::endl;
            // mover->velocityHeading += glm::vec3(-1,0,0);
            buf[1] = 1;
            break;

        case GLFW_KEY_S:
            // Close the window. This causes the program to also terminate.
            std::cout << "Movin back!" << std::endl;
            // mover->velocityHeading += glm::vec3(0,0,1);
            buf[2] = 1;
            break;

        case GLFW_KEY_D:
            // Close the window. This causes the program to also terminate.
            std::cout << "Movin right!" << std::endl;
            // mover->velocityHeading += glm::vec3(1,0,0);
            buf[3] = 1;
            break;

        default:
            break;
        }
    }

    if (action == GLFW_RELEASE) {
        switch (key) {
        case GLFW_KEY_W:
            // Close the window. This causes the program to also terminate.
            std::cout << "Stopping forward!" << std::endl;
            // mover->velocityHeading -= glm::vec3(0,0,-1);
            buf[0] = 0;
            break;

        case GLFW_KEY_A:
            // Close the window. This causes the program to also terminate.
            std::cout << "Stopping left!" << std::endl;
            // mover->velocityHeading -= glm::vec3(-1,0,0);
            buf[1] = 0;
            break;

        case GLFW_KEY_S:
            // Close the window. This causes the program to also terminate.
            std::cout << "Stopping back!" << std::endl;
            // mover->velocityHeading -= glm::vec3(0,0,1);
            buf[2] = 0;
            break;

        case GLFW_KEY_D:
            // Close the window. This causes the program to also terminate.
            std::cout << "Stopping right!" << std::endl;
            // mover->velocityHeading -= glm::vec3(1,0,0);
            buf[3] = 0;
            break;

        default:
            break;
        }
    }

    Packet* packet = new Packet();
    packet->write_int((int)PacketType::PLAYER_INPUT);
    for (int i = 0; i < 4; i++) {
        packet->write_byte(buf[i]);
    }
    client->send(packet);
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