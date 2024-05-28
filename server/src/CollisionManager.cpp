#include <iostream>
#include <cmath>
#include <list>

#include "CollisionManager.hpp"
#include "NetTransform.hpp"
#include "Transform.hpp"

// four enemy attacks: 
// 1. jump and stomp, shockwave expanding 
// 2. mark players and cause damage
// 3. 4 lasers swiping
// 4. normal attack: swipe hit

void CollisionManager::add(GameObject* owner) {
    std::lock_guard<std::mutex> lock(_mutex);
    Collider* collider = owner->GetComponent<Collider>();
    if (!collider->GetIsPoint() && !collider->GetIsSector()) {
        colliderOwners[collider] = owner;
    }
}

void CollisionManager::remove(GameObject* owner) {
    std::lock_guard<std::mutex> lock(_mutex);
    Collider* collider = owner->GetComponent<Collider>();
    if (!collider->GetIsPoint() && !collider->GetIsSector()) {
        colliderOwners.erase(collider);
    }
}

// return true if player attack hits
bool CollisionManager::movePlayerAttack(GameObject* owner, GameObject* target, glm::vec3 newPosition) {
    Collider* attCollider = owner->GetComponent<Collider>();
    Collider* targetCollider = target->GetComponent<Collider>();
    attCollider->SetPosition(newPosition);
    owner->GetComponent<NetTransform>()->SetPosition(newPosition);
    return collisionCylinderPoint(targetCollider, attCollider);
}

// return a list of GameObjects that boss swipe hits (swiping lasers, swiping attack)
std::vector<GameObject*> CollisionManager::moveBossSwipe(Collider* attCollider, float amount) {
    std::lock_guard<std::mutex> lock(_mutex);
    std::vector<GameObject*> hitObjects;

    float oldCenterAngle = (attCollider->GetStartAngle() + attCollider->GetEndAngle())/2;
    attCollider->SetStartAngle(attCollider->GetStartAngle() + amount);
    attCollider->SetEndAngle(attCollider->GetEndAngle() + amount);

    for (const auto& pair : colliderOwners) {
        if (collisionCylinderSector(pair.first, attCollider) 
            && invincibles.find(pair.second) == invincibles.end()) {
            hitObjects.push_back(pair.second);
        }
    }
    return hitObjects;
}

// return a list of GameObjects that shockwave hits
std::vector<GameObject*> CollisionManager::moveBossShockwave(GameObject* owner, float newRadius) {
    std::lock_guard<std::mutex> lock(_mutex);
    Collider* attCollider = owner->GetComponent<Collider>();
    NetTransform* attTransform = owner->GetComponent<NetTransform>();
    std::vector<GameObject*> hitObjects;

    attCollider->SetRadius(newRadius);
    attTransform->SetScale(glm::vec3(newRadius, attTransform->GetScale().y, newRadius));

    for (const auto& pair : colliderOwners) {
        if (collisionCylinderCylinder(pair.first, attCollider) 
            && invincibles.find(pair.second) == invincibles.end()) {
            hitObjects.push_back(pair.second);
        }
    }
    return hitObjects;
}

// return a list of GameObjects that mark-attack hits
std::vector<GameObject*> CollisionManager::moveBossMark(GameObject* owner) {
    std::lock_guard<std::mutex> lock(_mutex);
    Collider* attCollider = owner->GetComponent<Collider>();
    std::vector<GameObject*> hitObjects;

    for (const auto& pair : colliderOwners) {
        if (collisionCylinderCylinder(pair.first, attCollider)
            && invincibles.find(pair.second) == invincibles.end()) {
            hitObjects.push_back(pair.second);
        }
    }
    return hitObjects;
}

bool CollisionManager::move(GameObject* owner, glm::vec3 newPosition) {
    std::lock_guard<std::mutex> lock(
        _mutex); // checkCollisionCylinder touches colliderOwners
    Collider* collider = owner->GetComponent<Collider>();
    collider->SetPosition(newPosition);
    // if collision, revert to previous collider
    if (checkCollisionCylinder(collider)) {
        if (owner->GetComponent<NetTransform>() != nullptr) {
            collider->SetPosition(owner->GetComponent<NetTransform>()->GetPosition());
        } else {
            collider->SetPosition(owner->GetComponent<Transform>()->GetPosition());
        }
        return true;
    } else { // otherwise, update transform
        if (owner->GetComponent<NetTransform>() != nullptr) {
            owner->GetComponent<NetTransform>()->SetPosition(newPosition);
        } else {
            owner->GetComponent<Transform>()->SetPosition(newPosition);
        }
        return false;
    }
}

void CollisionManager::setInvincible(GameObject* owner) {
    std::lock_guard<std::mutex> lock(inv_mutex);
    invincibles.insert(owner);
}

void CollisionManager::unsetInvincible(GameObject* owner) {
    std::lock_guard<std::mutex> lock(inv_mutex);
    invincibles.erase(owner);
}

bool CollisionManager::collisionCylinderCylinder(const Collider* cyl1,
                                                 const Collider* cyl2) {
    glm::vec3 position1 = cyl1->GetPosition();
    glm::vec3 position2 = cyl2->GetPosition();
    // First check for overlap in the z-axis
    float z1Min = position1.y;
    float z1Max = position1.y + cyl1->GetHeight();
    float z2Min = position2.y;
    float z2Max = position2.y + cyl2->GetHeight();
    if (z1Max < z2Min || z2Max < z1Min)
        return false;

    // Then check for overlap in the x-y plane
    float dx = position1.x - position2.x;
    float dy = position1.z - position2.z;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = cyl1->GetRadius() + cyl2->GetRadius();
    if (distanceSquared >= radiusSum * radiusSum)
        return false;

    return true;
}

bool CollisionManager::collisionCylinderSector(const Collider* cyl,
                                               const Collider* sec) {
    glm::vec3 position1 = cyl->GetPosition();
    glm::vec3 position2 = sec->GetPosition();
    float z1Min = position1.y;
    float z1Max = position1.y + cyl->GetHeight();
    float z2Min = position2.y;
    float z2Max = position2.y + sec->GetHeight();
    if (z1Max < z2Min || z2Max < z1Min)
        return false;

    Vector2 edge1 = {sec->GetRadius() * cos(sec->GetStartAngle()),
                     sec->GetRadius() * sin(sec->GetStartAngle())};
    Vector2 edge2 = {sec->GetRadius() * cos(sec->GetEndAngle()),
                     sec->GetRadius() * sin(sec->GetEndAngle())};
    Vector2 conn = {position1.x - position2.x, position1.z - position2.z};
    if (conn.isBetween(edge1, edge2)) {
        if (sqrt(conn.dot(conn)) < cyl->GetRadius() + sec->GetRadius()) {
            return true;
        } else {
            return false;
        }
    } else {
        Vector2 edges[2] = {edge1, edge2};
        for (int i = 0; i < 2; i++) {
            float edgeLength = sqrt(edges[i].dot(edges[i]));
            Vector2 edgeUnit = edges[i].normalize();
            float projectedLength = ((position1.x - position2.x) * edgeUnit.x +
                                     (position1.z - position2.z) * edgeUnit.z);
            Vector2 closestPoint;
            if (projectedLength < 0) {
                closestPoint = {position2.x, position2.z};
            } else if (projectedLength > edgeLength) {
                closestPoint = {position2.x + edges[i].x,
                                position2.z + edges[i].z};
            } else {
                closestPoint = {position2.x + projectedLength * edgeUnit.x,
                                position2.z + projectedLength * edgeUnit.z};
            }
            if ((closestPoint.x - position1.x) *
                        (closestPoint.x - position1.x) +
                    (closestPoint.z - position1.z) *
                        (closestPoint.z - position1.z) <
                cyl->GetRadius() * cyl->GetRadius())
                return true;
        }
        return false;
    }
}

bool CollisionManager::collisionCylinderPoint(const Collider* cyl,
                                              const Collider* point) {
    glm::vec3 position1 = cyl->GetPosition();
    glm::vec3 position2 = point->GetPosition();
    float zMin = position1.y;
    float zMax = position1.y + cyl->GetHeight();
    if (position2.y < zMin || position2.y > zMax)
        return false;
    float dx = position2.x - position1.x;
    float dy = position2.z - position1.z;
    float distanceSquared = dx * dx + dy * dy;
    if (distanceSquared >= cyl->GetRadius() * cyl->GetRadius())
        return false;
    return true;
}

// Not used right now
bool CollisionManager::collisionCylinderBoundary(const Collider* cyl) {
    glm::vec3 position1 = cyl->GetPosition();
    if (position1.x - cyl->GetRadius() < 0 ||
        position1.x + cyl->GetRadius() > BOUNDARY_LEN ||
        position1.z - cyl->GetRadius() < 0 ||
        position1.z + cyl->GetRadius() > BOUNDARY_LEN)
        return true;
    return false;
}

bool CollisionManager::checkCollisionCylinder(Collider* cyl) {
    GameObject* cylOwner = colliderOwners.at(cyl);
    for (const auto& pair : colliderOwners) {
        if (cylOwner != pair.second) {
            if (!pair.first->GetIsSector() && !pair.first->GetIsPoint() &&
                collisionCylinderCylinder(cyl, pair.first))
                return true;
        }
    }
    return false;
}
