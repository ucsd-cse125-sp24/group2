#ifndef PLAYER_H
#define PLAYER_H
#include "glm/glm.hpp"
#include "entity.hpp"

class Player : public Entity {
   public:
    glm::vec2 inputs;
    virtual void update() override;
};

#endif  // PLAYER_H