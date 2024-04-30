#include "Window.h"
#include "Client.h"
#include "core.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "GameManager.hpp"
#include <glm/gtx/string_cast.hpp>

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
    glfwSetKeyCallback(window, Window::keyCallback);

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

int main(void) {
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

    client.setCallback([window](Packet* params) {
        GameManager::instance().handle_packet(params);
    });
    client.connect("127.0.0.1", 25565);

    // Loop while GLFW window should stay open.
    while (!glfwWindowShouldClose(window)) {
        // Main render display callback. Rendering of objects is done here.
        Window::displayCallback(window);

        // Idle callback. Updating objects, etc. can be done here.
        Window::idleCallback();
    }

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}