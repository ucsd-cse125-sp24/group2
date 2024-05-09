#include "Window.h"
#include "Client.h"
#include "core.h"
#include "InputManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GameManager.hpp"
#include <glm/gtx/string_cast.hpp>
#include "GameObject.hpp"
#include "Transform.hpp"
#include "ConcurrentQueue.hpp"
#include <thread>
#include "AudioManager.hpp"

ConcurrentQueue<std::function<void(void)>> task_queue;

void error_callback(int error, const char* description) {
    // Print error.
    std::cerr << description << std::endl;
}

void setup_callbacks(GLFWwindow* window) {
    // Set the error callback.
    glfwSetErrorCallback(error_callback);
    // Set the window resize callback.
    glfwSetWindowSizeCallback(window, Window::resizeCallback);

    // Set the key callback.
    // glfwSetKeyCallback(window, Window::keyCallback);
    InputManager::setUpCallback(window);
    // Set the mouse and cursor callbacks
    glfwSetMouseButtonCallback(window, Window::mouse_callback);
    glfwSetCursorPosCallback(window, Window::cursor_callback);
}

void setup_opengl_settings() {
    // Enable depth buffering.
    glEnable(GL_DEPTH_TEST);
    // Related to shaders and z value comparisons for the depth buffer.
    glDepthFunc(GL_LEQUAL);
    // Set polygon drawing mode to fill front and back of each polygon.
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // Set clear color to black.
    glClearColor(0.0, 0.0, 0.0, 0.0);
}

void print_versions() {
    // Get info of GPU and supported OpenGL version.
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version supported: " << glGetString(GL_VERSION)
              << std::endl;

    // If the shading language symbol is defined.
#ifdef GL_SHADING_LANGUAGE_VERSION
    std::cout << "Supported GLSL version is: "
              << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
#endif
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./menv [IP] [PORT]\n");
        return 1;
    }
    Client client;

    // Create the GLFW window.
    GLFWwindow* window = Window::createWindow(800, 600);
    if (!window)
        exit(EXIT_FAILURE);

    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // Setup OpenGL settings.
    setup_opengl_settings();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram(client))
        exit(EXIT_FAILURE);
    // Initialize objects/pointers for rendering; exit if initialization fails.
    if (!Window::initializeObjects())
        exit(EXIT_FAILURE);

    client.setCallback([&](Packet* params) {
        task_queue.push_back(
            [&, params]() { GameManager::instance().handle_packet(params); });
    });
    client.connect(argv[1], atoi(argv[2]));

    AudioManager::instance().addNote("../assets/audio/Fsharp.wav", 'i');
    AudioManager::instance().addNote("../assets/audio/Gsharp.wav", 'j');
    AudioManager::instance().addNote("../assets/audio/A.wav", 'k');
    AudioManager::instance().addNote("../assets/audio/Csharp.wav", 'l');

    AudioManager::instance().setMain(
        "../assets/audio/041124_futuristic_01_rough_mix.mp3", 1.0f);
    AudioManager::instance().setBpm(112);
    AudioManager::instance().play();

    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window)) {
        // Main render display callback. Rendering of objects is done here.
        Window::displayCallback(window);

        AudioManager::instance().update();

        // Idle callback. Updating objects, etc. can be done here.
        Window::idleCallback();
        while (!task_queue.empty()) {
            task_queue.pop_front()();
        }
    }

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}