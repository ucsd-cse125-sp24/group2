#include "../include/GameObject.hpp"
#include "../include/Component.hpp"
#include "../include/Transform.hpp"


// FUNCTION DEFINITIONS

GameObject::GameObject() {
    Transform* transform = new Transform(glm::vec3(), glm::vec3(), glm::vec3());
    addComponent(transform);
}

GameObject::GameObject(glm::vec3 pos = glm::vec3(), glm::vec3 rot = glm::vec3(), glm::vec3 scale = glm::vec3()) {
    Transform* transform = new Transform(pos, rot, scale);
    addComponent(transform);
}

GameObject::~GameObject() {
    for (IComponent* component : components) {
        delete component;
    }
}