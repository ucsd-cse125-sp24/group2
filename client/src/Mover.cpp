#include "Mover.h"

Mover::Mover() : INetworkComponent() {
    position = glm::vec3(0);
    velocityHeading = glm::vec3(0);
    speed = 0.2;
}

Mover::Mover(std::string path) : INetworkComponent() {
    position = glm::vec3(0);
    velocityHeading = glm::vec3(0);
    speed = 0.2;
    model = new Model(path);
    clip = new AnimationClip(path, model);
    animationPlayer = new AnimationPlayer(clip);
}

Mover::~Mover() { delete cube; }

void Mover::Update(float deltaTime) {
    model->update(deltaTime, position);
    animationPlayer->update(deltaTime);
}

void Mover::UpdatePhysics(float deltaTime) {
    glm::vec3 normalizedVelocityHeading = velocityHeading;
    if (velocityHeading != glm::vec3(0)) {
        normalizedVelocityHeading = glm::normalize(velocityHeading);
    }
    position += normalizedVelocityHeading * speed;
}

void Mover::Draw(glm::mat4 view, GLuint shaderProgram) {
    // glm::mat4 transform = glm::translate(position);
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

std::string Mover::ToString() {
    return "Mover";
}