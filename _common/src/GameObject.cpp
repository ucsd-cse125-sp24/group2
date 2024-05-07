#include "GameObject.hpp"
#include "Transform.hpp"
#include "Collider.hpp"
#include <algorithm>


GameObject::GameObject() {
    Transform* transform = new Transform(this);
    Collider* collider = new Collider(this);
    AddComponent(transform);
    AddComponent(collider);
}

GameObject::GameObject(glm::vec3 newPosition, glm::vec3 newRotation,
                       glm::vec3 newScale) {
    Transform* transform =
        new Transform(this, newPosition, newRotation, newScale);
    Collider* collider = 
        new Collider(this, newPosition, newRotation, newScale);
    AddComponent(transform);
    AddComponent(collider);
}

void GameObject::AddComponent(IComponent* newComponent) {
    components.push_back(newComponent);
    typeToComponentMap.emplace(typeid(*(newComponent)), newComponent);
}

void GameObject::RemoveComponent(IComponent* comp) {
    std::remove(components.begin(), components.end(), comp);
    typeToComponentMap.erase(typeid(*comp));
}

std::string GameObject::ToString() {
    return "GameObject";
}