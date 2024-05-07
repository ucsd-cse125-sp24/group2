#include "GameObject.hpp"
#include "Transform.hpp"
#include "Collider.hpp"
#include "CollisionManager.hpp"
#include <algorithm>

// To discuss: don't we need something to remove a GameObject?
GameObject::GameObject() {
    Transform* transform = new Transform(this);
    Collider* collider = new Collider(this);
    CollisionManager::instance().add(this);
    AddComponent(transform);
    AddComponent(collider);
}

GameObject::GameObject(glm::vec3 newPosition, glm::vec3 newRotation,
                       glm::vec3 newScale) {
    Transform* transform =
        new Transform(this, newPosition, newRotation, newScale);
    Collider* collider = 
        new Collider(this, newPosition, newRotation, newScale);
    CollisionManager::instance().add(this);
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