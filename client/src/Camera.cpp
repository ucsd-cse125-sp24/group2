////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

Camera::Camera() { Reset(); }
void Camera::Update() {
    // Compute camera world matrix
    glm::mat4 world(1);
    world[3] = glm::vec4(position, 1);

    glm::mat4 rotationMatrix = glm::orientate4(glm::vec3(-0.2, 0, 0));
    world[0] = rotationMatrix[0];
    world[1] = rotationMatrix[1];
    world[2] = rotationMatrix[2];

    // Compute view matrix (inverse of world matrix)
    // glm::mat4 view = glm::inverse(world);
    glm::mat4 view = glm::lookAt(position, target, glm::vec3(0, 1, 0));

    // Compute perspective projection matrix
    glm::mat4 project =
        glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

    // Compute final view-projection matrix
    ViewProjectMtx = project * view;
}
void Camera::Reset() {
    FOV = 45.0f;
    Aspect = 1.33f;
    NearClip = 0.1f;
    FarClip = 10000.0f;

    Distance = 500.0f;
    Azimuth = 0.0f;
    Incline = 20.0f;
}