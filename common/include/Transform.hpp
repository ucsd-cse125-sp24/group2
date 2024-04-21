#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "../glm/glm.hpp"
#include "Component.hpp"


class Transform : public IComponent {
    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;

public:
    Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);

    void receive(void* value);
    
    glm::vec3 getPos() { return pos; }
    glm::vec3 getRot() { return rot; }
    glm::vec3 getScale() { return scale; }

    void setPos(glm::vec3 inputPos) { pos = inputPos; }
    void setRot(glm::vec3 inputRot) { rot = inputRot; }
    void setScale(glm::vec3 inputScale) { scale = inputScale; }
};


// FUNCTION DEFINITIONS

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
}

void Transform::receive(void* value) {
    glm::vec3* newPosition = reinterpret_cast<glm::vec3*>(value);
    setPos(*newPosition);
}


#endif // TRANSFORM_H