////////////////////////////////////////
// Camera.h
////////////////////////////////////////

#pragma once

#include "core.h"
#include "glm/gtx/euler_angles.hpp"

// The Camera class provides a simple means to controlling the 3D camera. It
// could be extended to support more interactive controls. Ultimately. the
// camera sets the GL projection and viewing matrices.

class Camera {
public:
    glm::vec3 position = glm::vec3();
    glm::vec3 basePosition = glm::vec3();
    glm::vec3 cameraShakeOffset = glm::vec3();
    float cameraShakeTime = 0.0f;
    float shakePowerX = 1.0f;
    float shakePowerY = 1.0f;
    Camera();

    void Update(float deltaTime);
    void Reset();

    // Access functions
    void SetAspect(float a) { Aspect = a; }
    void SetDistance(float d) { Distance = d; }
    void SetAzimuth(float a) { Azimuth = a; }
    void SetIncline(float i) { Incline = i; }
    void SetPosition(glm::vec3 pos) { position = pos; }
    void SetTarget(glm::vec3 t) { target = t; }

    void Shake(float time, float powerX, float powerY) {
        cameraShakeTime = time;
        shakePowerX = powerX;
        shakePowerY = powerY;
    }

    float GetDistance() { return Distance; }
    float GetAzimuth() { return Azimuth; }
    float GetIncline() { return Incline; }
    glm::vec3 GetTarget() { return target; }
    float GetAspect() { return Aspect; }

    const glm::mat4& GetViewProjectMtx() { return ViewProjectMtx; }
    const glm::mat4& GetViewMtx() { return viewMtx; }
    const glm::mat4& GetProjMtx() { return projMtx; }

private:
    // Perspective controls
    float FOV;      // Field of View Angle (degrees)
    float Aspect;   // Aspect Ratio
    float NearClip; // Near clipping plane distance
    float FarClip;  // Far clipping plane distance

    // Polar controls
    float
        Distance;  // Distance of the camera eye position to the origin (meters)
    float Azimuth; // Rotation of the camera eye position around the Y axis
                   // (degrees)
    float
        Incline; // Angle of the camera eye position over the XZ plane (degrees)

    // camera transform?
    glm::vec3 target = glm::vec3();

    // Computed data
    glm::mat4 ViewProjectMtx;
    glm::mat4 viewMtx;
    glm::mat4 projMtx;
};