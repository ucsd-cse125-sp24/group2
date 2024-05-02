#include "GameObject.hpp"
#include "Transform.hpp"
#include "Collider.hpp"

GameObject::GameObject() {
    Transform* transform = new Transform(this);
    Collider* collider = new Collider(this);
    addComponent(transform);
    addComponent(collider);
}

GameObject::GameObject(glm::vec3 newPosition, glm::vec3 newRotation,
                       glm::vec3 newScale) {
    Transform* transform =
        new Transform(this, newPosition, newRotation, newScale);
    Collider* collider = 
        new Collider(this, newPosition, newRotation, newScale);
    addComponent(transform);
    addComponent(collider);
}

void GameObject::addComponent(IComponent* newComponent) {
    components.push_back(newComponent);
    typeToComponentMap.emplace(typeid(*(newComponent)), newComponent);
}

std::string GameObject::toString() {
    return "GameObject with transform: " +
           typeToComponentMap[typeid(Transform)]->toString();
}