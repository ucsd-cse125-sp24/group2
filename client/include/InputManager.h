#pragma once
#include "core.h"
#include <unordered_map>

typedef struct MousePosition {
    float x;
    float y;
} MousePosition;

class InputManager {
public:
    static MousePosition mousePosition;
    static bool isKeyPressed(int key);
    static void setUpCallback(GLFWwindow* window);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void setDefaultKeys();
    static void CursorCallback(GLFWwindow* window, double currX, double currY);
private:
    static std::unordered_map<int, bool> keys;
    // static GLFWwindow* window;
};