#include "../include/NetTransform.hpp"
#include "../glm/gtx/string_cast.hpp"


NetTransform::NetTransform() :
    INetworkComponent(),
    position(glm::vec3()),
    rotation(glm::vec3()),
    scale(glm::vec3())
{}

NetTransform::NetTransform(NetworkObject* owner) : 
    INetworkComponent(owner),
    position(glm::vec3()),
    rotation(glm::vec3()),
    scale(glm::vec3())
{}

NetTransform::NetTransform(NetworkObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale) : 
    INetworkComponent(owner),
    position(newPosition),
    rotation(newRotation),
    scale(newScale)
{}

std::string NetTransform::ToString() {
    return 
        "Networked Transform: pos(" + glm::to_string(position) +
        ") rot(" + glm::to_string(position) +
        ") scale(" + glm::to_string(position) + ")";
}