#include "Player.hpp"

// #include "../include/core.h"
// #define GLEW_STATIC
// #include "../include/GL/glew.h"

#include "../include/Mover.hpp"
#include "../include/Model.h"
#include "../include/AnimationClip.h"
#include "../include/AnimationPlayer.h"


// Player::Player() : Entity() {
//     Mover* mover = new Mover(this);
//     AddComponent(mover);
//     Model* model = new Model(this);
//     AddComponent(model);
//     AnimationClip* clip = new AnimationClip(this);
//     AddComponent(clip);
//     AnimationPlayer* animPlayer = new AnimationPlayer(this);
//     AddComponent(animPlayer);
// }

Player::Player(std::string path) : Entity() {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    Model* model = new Model(this, path);
    AddComponent(model);
    AnimationClip* animationClip = new AnimationClip(this, path, model);
    AddComponent(animationClip);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, animationClip);
    AddComponent(animationPlayer);
}

void Player::update() {}

void Player::UpdateModel(float deltaTime) {
    Model* model = GetComponent<Model>();
    AnimationPlayer* animationPlayer = GetComponent<AnimationPlayer>();
    NetTransform* netTransform = GetComponent<NetTransform>();
    
    model->update(deltaTime, netTransform->position);
    animationPlayer->update(deltaTime);
}

void Player::Draw(glm::mat4 view, GLuint shaderProgram) {
    Model* model = GetComponent<Model>();
    AnimationPlayer* animationPlayer = GetComponent<AnimationPlayer>();


    model->draw(view, shaderProgram);
    auto transforms = animationPlayer->getFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); i++) {
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(
            glGetUniformLocation(
                shaderProgram,
                ("finalBonesMatrices[" + std::to_string(i) + "]").c_str()),
            1, GL_FALSE, glm::value_ptr(transforms[i]));
    }
}