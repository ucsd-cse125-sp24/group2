#include "../include/Transform.hpp"
#include "../glm/gtx/string_cast.hpp"

Transform::Transform()
    : IComponent(), position(glm::vec3()), rotation(glm::vec3()),
      scale(glm::vec3(1, 1, 1)) {}

Transform::Transform(GameObject* owner)
    : IComponent(owner), position(glm::vec3()), rotation(glm::vec3()),
      scale(glm::vec3(1, 1, 1)) {}

Transform::Transform(GameObject* owner, glm::vec3 newPosition,
                     glm::vec3 newRotation, glm::vec3 newScale)
    : IComponent(owner), position(newPosition), rotation(newRotation),
      scale(newScale) {}

std::string Transform::ToString() {
    return "Transform: pos(" + glm::to_string(position) + ") rot(" +
           glm::to_string(position) + ") scale(" + glm::to_string(position) +
           ")";
}