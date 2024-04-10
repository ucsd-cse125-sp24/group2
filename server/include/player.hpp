#ifndef PLAYER_H
#define PLAYER_H
#include "glm/glm.hpp"
#include "entity.hpp"

class Player : public Entity {
   public:
    glm::vec2 inputs;
    virtual void update() {
        position += glm::vec3(inputs.x, 0, inputs.y);
    }
};

#endif // PLAYER_H