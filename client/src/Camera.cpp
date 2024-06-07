////////////////////////////////////////
// Camera.cpp
////////////////////////////////////////

#include "Camera.h"

Camera::Camera() { Reset(); }
void Camera::Update(float deltaTime) {
    if (cameraShakeTime > 0) {
        float rx = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        float ry = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
        glm::vec3 right = glm::normalize(
            glm::cross(target - basePosition, glm::vec3(0, 1, 0)));
        cameraShakeOffset =
            glm::vec3(0, ry * shakePowerY, 0) + right * rx * shakePowerX;
    }

    position = basePosition + cameraShakeOffset;

    viewMtx = glm::lookAt(position, target, glm::vec3(0, 1, 0));

    // Compute perspective projection matrix
    projMtx = glm::perspective(glm::radians(FOV), Aspect, NearClip, FarClip);

    // Compute final view-projection matrix
    ViewProjectMtx = projMtx * viewMtx;

    cameraShakeTime -= deltaTime;
    if (cameraShakeTime < 0) {
        cameraShakeTime = 0;
        cameraShakeOffset = glm::vec3();
    }
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