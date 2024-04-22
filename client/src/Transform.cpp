#include "../include/Transform.hpp"
#include <string>


// FUNCTION DEFINITIONS

Transform::Transform(glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    this->pos = pos;
    this->rot = rot;
    this->scale = scale;
}

void Transform::receive(Topic topic, void* value) {
    std::cerr << "Temporary testing solution" << std::endl;
    // How can I make this so that it can differntiate between all the different vectors and data?
    // glm::vec3* newPosition = reinterpret_cast<glm::vec3*>(value);
    // setPos(*newPosition);

    glm::vec3* vector = reinterpret_cast<glm::vec3*>(value);
    switch (topic) {
        case testPosition:
            setPos(*vector);
            break;
        case testRotation:
            setRot(*vector);
            break;
        case testScale:
            setScale(*vector);
            break;
        default:
            std::cerr << "Transform::recieve() invalid topic." << std::endl;
            break;
    }
}

std::string Transform::toString() {
    return "Position: " + glm::to_string(pos) 
        + ", Rotation: " + glm::to_string(rot)
        + ", Scale: " + glm::to_string(scale);
}