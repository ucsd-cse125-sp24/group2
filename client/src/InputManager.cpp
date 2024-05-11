#include "InputManager.h"
#include "ColorCodes.hpp"

std::unordered_map<int, bool> InputManager::keys =
    std::unordered_map<int, bool>();
MousePosition InputManager::mousePosition;

void InputManager::setDefaultKeys() {
    keys[GLFW_KEY_W] = false;
    keys[GLFW_KEY_A] = false;
    keys[GLFW_KEY_S] = false;
    keys[GLFW_KEY_D] = false;
    keys[GLFW_KEY_J] = false;
    keys[GLFW_KEY_I] = false;
    keys[GLFW_KEY_K] = false;
    keys[GLFW_KEY_L] = false;
}

void InputManager::setUpCallback(GLFWwindow* window) {
    // Set the key callback.
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, CursorCallback);
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

void InputManager::CursorCallback(GLFWwindow* window, double currX, double currY) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    mousePosition.x = currX / width;
    mousePosition.y = currY / height;
}

bool InputManager::isKeyPressed(int key) { return keys[key]; }