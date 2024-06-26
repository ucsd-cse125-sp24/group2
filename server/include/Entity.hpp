#ifndef ENTITY_H
#define ENTITY_H
#include "glm/glm.hpp"

class Entity {
   public:
    int network_id;
    glm::vec3 position = glm::vec3();
    virtual void update() = 0;
};

#endif  // ENTITY_H