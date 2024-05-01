#pragma once
#include "core.h"
#include <unordered_map>

class InputManager {
public:
    static bool isKeyPressed(int key);
    static void setUpCallback(GLFWwindow* window);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void setDefaultKeys();
private:
    static std::unordered_map<int, bool> keys;
    // static GLFWwindow* window;
};