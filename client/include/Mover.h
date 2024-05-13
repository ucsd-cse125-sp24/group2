#pragma once


#include <iostream>
#include "core.h"

#include "INetworkComponent.hpp"
#include "Cube.h"
#include "InputManager.h"
#include "Model.h"
#include "AnimationPlayer.h"
#include "AnimationClip.h"


class Mover : public INetworkComponent {
public:
    glm::vec3& position;
    glm::vec3 velocityHeading;
    float speed;

    Cube* cube;
    Model* model;
    AnimationPlayer* animationPlayer;
    AnimationClip* clip;

    Mover(NetworkObject* owner);
    Mover(NetworkObject* owner, std::string path);
    ~Mover();

    // void Update(float deltaTime);
    // void UpdatePhysics(float deltaTime);
    void Draw(glm::mat4 view, GLuint shaderProgram);

    void Serialize(Packet* packet) override;
    void Deserialize(Packet* packet) override;

    std::string ToString() override;
};