#include <iostream>
#include <cmath>
#include <list>

#include "CollisionManager.hpp"



void CollisionManager::add(GameObject* owner){
    Collider* collider = owner->getComponent<Collider>();
    colliderOwners[collider] = owner;
}

void CollisionManager::remove(GameObject* owner) {
    Collider* collider = owner->getComponent<Collider>();
    colliderOwners.erase(collider);
}

void CollisionManager::move(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation, glm::vec3 newScale){
    Collider* collider = owner->getComponent<Collider>();
    Transform* transform = owner->getComponent<Transform>();
    if (!collider->getIsPoint() && !collider->getIsSector()){
        newPosition.z = std::max(newPosition.z, 0.0f); // cannot go below ground
        collider->setPosition(newPosition);
        collider->setRadius(newScale.x);
        collider->setHeight(newScale.z);
        collider->setRotation(newRotation);
        // if collision, revert to previous collider
        if (checkCollisionCylinder(collider)){
            collider->setPosition(transform->getPosition());
            collider->setRadius(transform->getScale().x);
            collider->setHeight(transform->getScale().z);
            collider->setRotation(transform->getRotation());
        } else { // otherwise update transform
            transform->setPosition(collider->getPosition());
            glm::vec3 newTransformScale(collider->getRadius(), collider->getRadius(), collider->getHeight());
            transform->setScale(newTransformScale);
            transform->setRotation(collider->getRotation());
        }
    }
}

bool CollisionManager::collisionCylinderCylinder(const Collider* cyl1, const Collider* cyl2) {
    glm::vec3 position1 = cyl1->getPosition();
    glm::vec3 position2 = cyl2->getPosition();
    // First check for overlap in the z-axis
    float z1Min = position1.z; float z1Max = position1.z + cyl1->getHeight();
    float z2Min = position2.z; float z2Max = position2.z + cyl2->getHeight();
    if (z1Max < z2Min || z2Max < z1Min)
        return false;

    // Then check for overlap in the x-y plane
    float dx = position1.x - position2.x;
    float dy = position1.y - position2.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = cyl1->getRadius() + cyl2->getRadius();
    if (distanceSquared >= radiusSum * radiusSum)
        return false;

    return true;
}

bool CollisionManager::collisionCylinderSector(const Collider* cyl, const Collider* sec) {
    glm::vec3 position1 = cyl->getPosition();
    glm::vec3 position2 = sec->getPosition();
    float z1Min = position1.z; float z1Max = position1.z + cyl->getHeight();
    float z2Min = position2.z; float z2Max = position2.z + sec->getHeight();
    if (z1Max < z2Min || z2Max < z1Min)
        return false;

    Vector2 edge1 = {sec->getRadius() * cos(sec->getStartAngle()), sec->getRadius() * sin(sec->getStartAngle())};
    Vector2 edge2 = {sec->getRadius() * cos(sec->getEndAngle()), sec->getRadius() * sin(sec->getEndAngle())};
    Vector2 conn = {position1.x - position2.x, position1.y - position2.y};
    if (isBetween(conn, edge1, edge2)) {
        if (sqrt(conn.dot(conn)) < cyl->getRadius() + sec->getRadius()) {
            return true;
        } else {
            return false;
        }
    } else {
        Vector2 edges[2] = {edge1, edge2};
        for (int i = 0; i < 2; i++) {
            float edgeLength = sqrt(edges[i].dot(edges[i]));
            Vector2 edgeUnit = edges[i].normalize();
            float projectedLength = ((position1.x - position2.x) * edgeUnit.x + (position1.y - position2.y) * edgeUnit.y);
            Vector2 closestPoint;
            if (projectedLength < 0) {
                closestPoint = {position2.x, position2.y};
            } else if (projectedLength > edgeLength) {
                closestPoint = {position2.x + edges[i].x, position2.y + edges[i].y};
            } else {
                closestPoint = {position2.x + projectedLength * edgeUnit.x,
                                position2.y + projectedLength * edgeUnit.y};
            }
            if ((closestPoint.x - position1.x)*(closestPoint.x - position1.x) + 
                (closestPoint.y - position1.y)*(closestPoint.y - position1.y) < 
                cyl->getRadius() * cyl->getRadius())
                return true;
        }
        return false;
    }
}

bool CollisionManager::collisionCylinderPoint(const Collider* cyl, const Collider* point) {
    glm::vec3 position1 = cyl->getPosition();
    glm::vec3 position2 = point->getPosition();
    float zMin = position1.z;
    float zMax = position1.z + cyl->getHeight();
    if (position2.z < zMin || position2.z > zMax)
        return false;
    float dx = position2.x - position1.x;
    float dy = position2.y - position1.y;
    float distanceSquared = dx * dx + dy * dy;
    if (distanceSquared >= cyl->getRadius() * cyl->getRadius())
        return false;
    return true;
}

bool CollisionManager::collisionCylinderBoundary(const Collider* cyl) {
    glm::vec3 position1 = cyl->getPosition();
    if (position1.x-cyl->getRadius() < 0 || position1.x+cyl->getRadius() > BOUNDARY_LEN || 
        position1.y-cyl->getRadius() < 0 || position1.y+cyl->getRadius() > BOUNDARY_LEN) 
        return true;
    return false;
}

bool CollisionManager::checkCollisionCylinder(Collider* cyl) {
    if (collisionCylinderBoundary(cyl))
        return true;
    GameObject* cylOwner = colliderOwners.at(cyl);
    for (const auto& pair : colliderOwners) {
        if (cylOwner != pair.second){
            if (!pair.first->getIsSector() && !pair.first->getIsPoint()
                && collisionCylinderCylinder(cyl, pair.first)) 
                return true;
        }
    }
    return false;
}

