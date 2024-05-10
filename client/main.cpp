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

    // Create the GLFW window.
    float width = 800;
    float height = 600;
    GLFWwindow* window = Window::createWindow(width, height);
    if (!window)
        exit(EXIT_FAILURE);

    // Setup camera
    GameManager::instance().cam = new Camera();
    GameManager::instance().cam->SetAspect(float(width) / float(height));

    // Print OpenGL and GLSL versions.
    print_versions();
    // Setup callbacks.
    setup_callbacks(window);
    // Setup OpenGL settings.
    setup_opengl_settings();

    // Initialize input
    InputManager::setDefaultKeys();

    // Initialize the shader program; exit if initialization fails.
    if (!Window::initializeProgram())
        exit(EXIT_FAILURE);

    GameManager::instance().client.setCallback([&](Packet* params) {
        task_queue.push_back(
            [&, params]() { GameManager::instance().handle_packet(params); });
    });
    GameManager::instance().client.connect(argv[1], atoi(argv[2]));

    AudioManager::instance().addNote("../assets/audio/Fsharp.wav", 'i');
    AudioManager::instance().addNote("../assets/audio/Gsharp.wav", 'j');
    AudioManager::instance().addNote("../assets/audio/A.wav", 'k');
    AudioManager::instance().addNote("../assets/audio/Csharp.wav", 'l');

    AudioManager::instance().setMain(
        "../assets/audio/futuristic01-112bpm-Gbm.mp3", 1.0f);
    AudioManager::instance().setBpm(112);
    AudioManager::instance().play();

    // Loop while GLFW window should stay open.
    float deltaTime = 0;
    float timer = 0;
    float currentTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        // Calculate deltaTime
        float newTime = glfwGetTime();
        deltaTime = newTime - currentTime;
        currentTime = newTime;

        AudioManager::instance().update();

        // send input
        timer += deltaTime;
        if (timer > 0.025f) {
            Packet* pkt = new Packet();
            pkt->write_int((int)PacketType::PLAYER_INPUT);
            uint8_t* buf = new uint8_t[5];
            buf[0] = InputManager::isKeyPressed(GLFW_KEY_W);
            buf[1] = InputManager::isKeyPressed(GLFW_KEY_A);
            buf[2] = InputManager::isKeyPressed(GLFW_KEY_S);
            buf[3] = InputManager::isKeyPressed(GLFW_KEY_D);
            buf[4] = InputManager::isKeyPressed(GLFW_KEY_LEFT_SHIFT) && (buf[0] || buf[1] || buf[2] || buf[3]);
            for (int i = 0; i < 5; i++) {
                pkt->write_byte(buf[i]);
            }
            GameManager::instance().client.send(pkt);
            delete[] buf;
            timer = 0;
        }

        // Main render display callback. Rendering of objects is done here.
        GameManager::instance().cam->Update();
        Window::Render(window, &GameManager::instance().scene,
                       GameManager::instance().cam, deltaTime);

        // Update loop
        while (!task_queue.empty()) {
            task_queue.pop_front()();
        }

        GameManager::instance().scene.Update();
    }

    Window::cleanUp();
    // Destroy the window.
    glfwDestroyWindow(window);
    // Terminate GLFW.
    glfwTerminate();

    exit(EXIT_SUCCESS);
}