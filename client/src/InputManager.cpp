#include "InputManager.h"

std::unordered_map<int, bool> InputManager::keys =
    std::unordered_map<int, bool>();

void InputManager::setDefaultKeys() {
    keys[GLFW_KEY_W] = false;
    keys[GLFW_KEY_A] = false;
    keys[GLFW_KEY_S] = false;
    keys[GLFW_KEY_D] = false;
}

void InputManager::setUpCallback(GLFWwindow* window) {
    // Set the key callback.
    glfwSetKeyCallback(window, keyCallback);
}

void InputManager::keyCallback(GLFWwindow* window, int key, int scancode,
                               int action, int mods) {
    if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (action == GLFW_PRESS) {
        keys[key] = true;
    }
    if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

bool InputManager::isKeyPressed(int key) { return keys[key]; }