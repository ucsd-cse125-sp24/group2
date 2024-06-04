#include "InputManager.h"
#include "ColorCodes.hpp"

std::unordered_map<int, InputManager::KeyState> InputManager::keys;
MousePosition InputManager::mousePosition;

void InputManager::setDefaultKeys() {
    keys[GLFW_KEY_W] = (KeyState)0;
    keys[GLFW_KEY_A] = (KeyState)0;
    keys[GLFW_KEY_S] = (KeyState)0;
    keys[GLFW_KEY_D] = (KeyState)0;
    keys[GLFW_KEY_J] = (KeyState)0;
    keys[GLFW_KEY_I] = (KeyState)0;
    keys[GLFW_KEY_K] = (KeyState)0;
    keys[GLFW_KEY_L] = (KeyState)0;
    keys[GLFW_KEY_SPACE] = (KeyState)0;
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
        keys[key] = (KeyState)(keys[key] | KeyState::PRESSED);
        keys[key] = (KeyState)(keys[key] | KeyState::DOWN);
    }
    if (action == GLFW_RELEASE) {
        keys[key] = (KeyState)(keys[key] & ~KeyState::PRESSED);
        keys[key] = (KeyState)(keys[key] & ~KeyState::DOWN);
    }
}

void InputManager::CursorCallback(GLFWwindow* window, double currX,
                                  double currY) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    mousePosition.x = currX / width;
    mousePosition.y = currY / height;
}

bool InputManager::IsKeyDown(int key) {
    return (keys[key] & KeyState::DOWN) > 0;
}

// FIXME if this function is called in multiple places, it won't work
bool InputManager::IsKeyPressed(int key) {
    if ((keys[key] & KeyState::PRESSED) > 0) {
        keys[key] = (KeyState)(keys[key] & ~KeyState::PRESSED);
        return true;
    }
    return false;
}