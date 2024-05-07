#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Entity.hpp"

class Player : public Entity {
public:
    glm::vec2 inputs = glm::vec2();
    glm::vec3 velocity = glm::vec3();
    float speed = 0.2f;
    virtual void update() override;
    int32_t TypeID() const override;
};