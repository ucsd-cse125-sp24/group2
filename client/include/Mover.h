#pragma once

#include "core.h"
#include "Cube.h"
#include "InputManager.h"
#include <iostream>
#include "Model.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"
#include "../../_common/include/INetworkComponent.hpp"

class Mover : INetworkComponent {
public:
    glm::vec3 position;
    glm::vec3 velocityHeading;
    float speed;

    Cube* cube;
    Model* model;
    AnimationPlayer* animationPlayer;
    AnimationClip* clip;

    Mover();
    Mover(std::string path);
    ~Mover();

    void Update(float deltaTime);
    void UpdatePhysics(float deltaTime);
    void Draw(glm::mat4 view, GLuint shaderProgram);

    std::string ToString() override;
};