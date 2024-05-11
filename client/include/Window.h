#pragma once

#include "Camera.h"
#include "Cube.h"
#include "Shader.h"
#include "core.h"
#include "InputManager.h"

#include "Mover.h"

#include "PlayerManager.hpp"
#include "Client.h"
#include "engine/Scene.hpp"
class Window {
public:
    // Window Properties
    static int width;
    static int height;
    static const char* windowTitle;

    // Act as Constructors and desctructors
    static bool initializeProgram();
    static void cleanUp();

    // for the Window
    static GLFWwindow* createWindow(int width, int height);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    // update and draw functions
    static void Render(GLFWwindow*, Scene*, Camera*, float);

    static void mouse_callback(GLFWwindow* window, int button, int action,
                               int mods);
};