#pragma once

#include "core.h"
#include "Cube.h"
#include "InputManager.h"
#include <iostream>

class Mover {
    public:
        glm::vec3 position;
        glm::vec3 velocityHeading;
        float speed;
        Cube* cube;
        uint8_t buf[4];
        Mover();
        ~Mover();
        void Update(float deltaTime);
        void UpdatePhysics(float deltaTime);
        void Draw(glm::mat4 view, GLuint shaderProgram);
};