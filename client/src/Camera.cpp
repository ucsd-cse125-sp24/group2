////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"
#include "GameManager.hpp"
#include "InputManager.h"

Camera::Camera() { Reset(); }
void Camera::Update() {
    // Compute camera world matrix
    std::cout << "Camera::position: " << glm::to_string(position) << " in Camera::Update()." << std::endl;

    glm::mat4 world(1);
    world[3] = glm::vec4(position, 1);

    glm::mat4 rotationMatrix = glm::orientate4(glm::vec3(-0.2, 0, 0));
    world[0] = rotationMatrix[0];
    world[1] = rotationMatrix[1];
    world[2] = rotationMatrix[2];

    // Compute view matrix (inverse of world matrix)
    glm::mat4 view = glm::inverse(world); // THE ACTUAL ORIGINAL
    // glm::mat4 view = world; 
    // glm::mat4 view = glm::translate(glm::mat4(1), -position);
    // glm::mat4 view = glm::mat4(1);

    // Compute perspective projection matrix
    glm::mat4 project =
        glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

    // Compute final view-projection matrix
    ViewProjectMtx = project * view;

    // std::cout << "END OF Camera::Update()" << std::endl;
}
void Camera::Reset() {
    FOV = 45.0f;
    Aspect = 1.33f;
    NearClip = 0.1f;
    FarClip = 5000.0f;

    Distance = 500.0f;
    Azimuth = 0.0f;
    Incline = 20.0f;
}