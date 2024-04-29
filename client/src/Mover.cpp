#include "Mover.h"


Mover::Mover() {
    position = glm::vec3(0);
    velocityHeading = glm::vec3(0);
    speed = 0.2;

    cube = new Cube(glm::vec3(-0.5, -0.5, -0.5), glm::vec3(0.5, 0.5, 0.5));
}

Mover::~Mover() {
    delete cube;
}

void Mover::Update(float deltaTime) {
    buf[0] = InputManager::isKeyPressed(GLFW_KEY_W);
    buf[1] = InputManager::isKeyPressed(GLFW_KEY_A);
    buf[2] = InputManager::isKeyPressed(GLFW_KEY_S);
    buf[3] = InputManager::isKeyPressed(GLFW_KEY_D);
    std::cout<<"buf[0]: "<<buf[0]<<std::endl;
    UpdatePhysics(deltaTime);
}

void Mover::UpdatePhysics(float deltaTime) {
    glm::vec3 normalizedVelocityHeading = velocityHeading;
    if (velocityHeading != glm::vec3(0)) {
        normalizedVelocityHeading = glm::normalize(velocityHeading);
    }
    position += normalizedVelocityHeading * speed;
}

void Mover::Draw(glm::mat4 view, GLuint shaderProgram) {
    glm::mat4 transform = glm::translate(position);
    cube->draw(view * transform, shaderProgram);
}