#include "../include/GameObject.hpp"
#include "../include/Transform.hpp"


GameObject::GameObject() {
    Transform* transform = new Transform(this);
    addComponent(transform);
}

GameObject::GameObject(glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale) {
    Transform* transform = new Transform(this, newPosition, newRotation, newScale);
    addComponent(transform);
}


void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
    typeToComponentMap.emplace(typeid(*(newComponent)), newComponent);
}

std::string GameObject::toString() {
    return "GameObject with transform: " + typeToComponentMap[typeid(Transform)]->toString();
}