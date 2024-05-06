#pragma once


#include <iostream>
#include "core.h"

#include "../../_common/include/INetworkComponent.hpp"

#include "Cube.h"
#include "InputManager.h"
#include "Model.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"


class Mover : public INetworkComponent {
public:
    glm::vec3 position;
    glm::vec3 velocityHeading;
    float speed;

    Cube* cube;
    // references to the other components
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