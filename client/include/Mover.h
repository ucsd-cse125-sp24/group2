#pragma once

#include "core.h"
#include "Cube.h"

class Mover {
    public:
        glm::vec3 position;
        glm::vec3 velocityHeading;
        float speed;

        Cube* cube;
        
        Mover();
        ~Mover();
        void Update(float deltaTime);
        void UpdatePhysics(float deltaTime);
        void Draw(glm::mat4 view, GLuint shaderProgram);
};