#include <iostream>

#include "../include/NetTransform.hpp"
#include "../glm/gtx/string_cast.hpp"

NetTransform::NetTransform()
    : INetworkComponent(), position(glm::vec3()), rotation(glm::vec3()),
      scale(glm::vec3()) {}

NetTransform::NetTransform(NetworkObject* owner)
    : INetworkComponent(owner), position(glm::vec3()), rotation(glm::vec3()),
      scale(glm::vec3()) {}

NetTransform::NetTransform(NetworkObject* owner, glm::vec3 newPosition,
                           glm::vec3 newRotation, glm::vec3 newScale)
    : INetworkComponent(owner), position(newPosition), rotation(newRotation),
      scale(newScale) {}

void NetTransform::Serialize(Packet* packet) {
    packet->write_vec3(position);
    packet->write_vec3(rotation);
}

void NetTransform::Deserialize(Packet* packet) {
    packet->read_vec3(&position);
    packet->read_vec3(&rotation);
}

std::string NetTransform::ToString() {
    return "Networked Transform: pos(" + glm::to_string(position) + ") rot(" +
           glm::to_string(position) + ") scale(" + glm::to_string(position) +
           ")";
}