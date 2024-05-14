#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#define GLEW_STATIC
#include "../../client/include/GL/glew.h"
#include "../../client/include/GLFW/glfw3.h"
// #include "../../client/include/
// #include "../../client/include/core.h"

#include "Entity.hpp"
#include "NetTransform.hpp"
#include "Mover.hpp"

class Player : public Entity {
public:
    // glm::vec2 inputs = glm::vec2();
    // glm::vec3 velocity = glm::vec3();
    // float speed = 0.2f;

    int32_t TypeID() const override { return PLAYER; }
    
    // Used by server
    Player();

    virtual void update() override;

    // Used by client
    Player(std::string path);

    void UpdateModel(float deltaTime);
    void Draw(glm::mat4 view, GLuint shaderProgram);
};