#include "Mover.hpp"
#include "NetTransform.hpp"
#include <iostream>
#include "glm/gtx/string_cast.hpp"

Mover::Mover(NetworkObject* owner) : 
    INetworkComponent(owner),
    position(owner->GetComponent<NetTransform>()->position),
    rotation(owner->GetComponent<NetTransform>()->rotation) 
{
    // TODO: handle multiple speeds
    speed = 0.2f;
}

void Mover::Update() {
    // std::cout << "Mover::Update()" << std::endl;
    // glm::vec3 forward;
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f); // up or +y
    // glm::vec3 right;

    glm::mat4 rotate = glm::mat4(1.0f);
    rotate = glm::rotate(rotate, rotation.y, up);

    // std::cout << "  Rotate: " << glm::to_string(rotate);
    
    // forward = rotate * glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);
    // right = glm::cross(forward, up);
    movementHeading = glm::vec3(input.x, 0, input.y);

    if (movementHeading != glm::vec3()) movementHeading = glm::normalize(movementHeading);
    
    // std::cout << "  Moving position by: " << glm::to_string(glm::vec3(rotate * glm::vec4(movementHeading, 0) * speed)) << std::endl;

    // std::cout << "Heading: " << glm::to_string(movementHeading) << std::endl;
    // std::cout << "Moving: " << glm::to_string(glm::vec3(rotate * glm::vec4(movementHeading, 0) * speed)) << std::endl;

    position += glm::vec3(rotate * glm::vec4(movementHeading, 0) * speed);
}

void Mover::Serialize(Packet* packet) {
    //packet->write_float(speed);
}

void Mover::Deserialize(Packet* packet) {
    //packet->read_float(&speed);
}

std::string Mover::ToString() {
    return "Mover";
}