#include "GameObject.hpp"
#include "Transform.hpp"
#include <algorithm>


GameObject::GameObject() {
    Transform* transform = new Transform(this); 
    AddComponent(transform);
}

GameObject::GameObject(glm::vec3 newPosition, glm::vec3 newRotation,
                       glm::vec3 newScale) {
    Transform* transform =
        new Transform(this, newPosition, newRotation, newScale);
    AddComponent(transform);
}

void GameObject::AddComponent(IComponent* newComponent) {
    components.push_back(newComponent);
    typeToComponentMap.emplace(typeid(*(newComponent)), newComponent);
    newComponent->SetOwner(this);
}

void GameObject::RemoveComponent(IComponent* comp) {
    std::remove(components.begin(), components.end(), comp);
    typeToComponentMap.erase(typeid(*comp));
    comp->SetOwner(nullptr);
}

std::string GameObject::ToString() {
    return "GameObject";
}