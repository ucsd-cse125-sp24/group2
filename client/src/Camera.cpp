////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

Camera::Camera() { Reset(); }
void Camera::Update() {
    viewMtx = glm::lookAt(position, target, glm::vec3(0, 1, 0));

    // Compute perspective projection matrix
    projMtx = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

    // Compute final view-projection matrix
    ViewProjectMtx = projMtx * viewMtx;
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