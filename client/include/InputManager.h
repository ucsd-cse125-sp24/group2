#pragma once
#include "core.h"
#include <unordered_map>

typedef struct MousePosition {
    float x;
    float y;
} MousePosition;

class InputManager {
private:
    enum KeyState : int {
        PRESSED = 1,
        DOWN = 2,
    };
    static std::unordered_map<int, KeyState> keys;

public:
    static MousePosition mousePosition;
    static bool IsKeyDown(int key);
    static bool IsKeyPressed(int key);
    static void setUpCallback(GLFWwindow* window);
    static void keyCallback(GLFWwindow* window, int key, int scancode,
                            int action, int mods);
    static void setDefaultKeys();
    static void CursorCallback(GLFWwindow* window, double currX, double currY);
};