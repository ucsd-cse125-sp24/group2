#pragma once
#include "Window.h"

class InputManager {
public:
    InputManager();
    InputManager(GLFWwindow* window);
private:
    GLFWwindow* window;
};