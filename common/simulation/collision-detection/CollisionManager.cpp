#include <iostream>
#include <cmath>
#include <list>
#include "Vector.h"

#include "CollisionManager.h"

Collider* CollisionManager::addCylinder(int owner, float x, float y, float z, float radius, float height){
    z = std::max(z, 0.0f); // cannot go below ground
    auto collider = std::make_unique<Collider>(false, false, owner, x, y, z, radius, height, 0, 0);
    colliders.push_back(std::move(collider));
    return colliders.back().get();
}

Collider* CollisionManager::addSector(int owner, float x, float y, float z, float radius, float height, float startAngle, float endAngle){
    auto collider = std::make_unique<Collider>(true, false, owner, x, y, z, radius, height, startAngle, endAngle);
    colliders.push_back(std::move(collider));
    return colliders.back().get();
}

Collider* CollisionManager::addPoint(int owner, float x, float y, float z){
    auto collider = std::make_unique<Collider>(false, true, owner, x, y, z, 0, 0, 0, 0);
    colliders.push_back(std::move(collider));
    return colliders.back().get();
}

void CollisionManager::removeCollider(Collider* collider) {
    colliders.erase(std::remove_if(colliders.begin(), colliders.end(),
        [collider](const std::unique_ptr<Collider>& item) { return item.get() == collider; }),
        colliders.end());
}

// may not need nradius or nheight if the model cannot change.
void CollisionManager::moveCylinder(Collider* collider, float nx, float ny, float nz, float nradius, float nheight) {
    if ((collider->isSector) || (collider->isPoint))
        std::cout << "Wrong collider type in moveCylinder";
    nz = std::max(nz, 0.0f); // cannot go below ground
    Collider temp = {false, false, collider->owner, nx, ny, nz, nradius, nheight, 0, 0};
    if (!checkCollisionCylinder(temp))
        collider->updateCylinder(nx, ny, nz, nradius, nheight);
}

void CollisionManager::moveSector(Collider* collider, float nx, float ny, float nz, float nradius, float nheight, float nstartAngle, float nendAngle) {
    collider->updateSector(nx, ny, nz, nradius, nheight, nstartAngle, nendAngle);
}

void CollisionManager::movePoint(Collider* collider, float nx, float ny, float nz) {
    collider->updatePoint(nx, ny, nz);
}

bool CollisionManager::collisionCylinderCylinder(const Collider& cyl1, const Collider& cyl2) {
    // First check for overlap in the z-axis
    float z1Min = cyl1.z; float z1Max = cyl1.z + cyl1.height;
    float z2Min = cyl2.z; float z2Max = cyl2.z + cyl2.height;
    if (z1Max < z2Min || z2Max < z1Min)
        return false;

    // Then check for overlap in the x-y plane
    float dx = cyl1.x - cyl2.x;
    float dy = cyl1.y - cyl2.y;
    float distanceSquared = dx * dx + dy * dy;
    float radiusSum = cyl1.radius + cyl2.radius;
    if (distanceSquared >= radiusSum * radiusSum)
        return false;

    return true;
}

bool CollisionManager::collisionCylinderSector(const Collider& cyl, const Collider& sec) {
    float z1Min = cyl.z; float z1Max = cyl.z + cyl.height;
    float z2Min = sec.z; float z2Max = sec.z + sec.height;
    if (z1Max < z2Min || z2Max < z1Min)
        return false;

    Vector2 edge1 = {sec.radius * cos(sec.startAngle), sec.radius * sin(sec.startAngle)};
    Vector2 edge2 = {sec.radius * cos(sec.endAngle), sec.radius * sin(sec.endAngle)};
    Vector2 conn = {cyl.x - sec.x, cyl.y - sec.y};
    if (isBetween(conn, edge1, edge2)) {
        if (sqrt(conn.dot(conn)) < cyl.radius + sec.radius) {
            return true;
        } else {
            return false;
        }
    } else {
        Vector2 edges[2] = {edge1, edge2};
        for (int i = 0; i < 2; i++) {
            float edgeLength = sqrt(edges[i].dot(edges[i]));
            Vector2 edgeUnit = edges[i].normalize();
            float projectedLength = ((cyl.x - sec.x) * edgeUnit.x + (cyl.y - sec.y) * edgeUnit.y);
            Vector2 closestPoint;
            if (projectedLength < 0) {
                closestPoint = {sec.x, sec.y};
            } else if (projectedLength > edgeLength) {
                closestPoint = {sec.x + edges[i].x, sec.y + edges[i].y};
            } else {
                closestPoint = {sec.x + projectedLength * edgeUnit.x,
                                sec.y + projectedLength * edgeUnit.y};
            }
            if ((closestPoint.x - cyl.x)*(closestPoint.x - cyl.x) + 
                (closestPoint.y - cyl.y)*(closestPoint.y - cyl.y) < 
                cyl.radius * cyl.radius)
                return true;
        }
        return false;
    }
}

bool CollisionManager::collisionCylinderPoint(const Collider& cyl, const Collider& point) {
    float zMin = cyl.z;
    float zMax = cyl.z + cyl.height;
    if (point.z < zMin || point.z > zMax)
        return false;
    float dx = point.x - cyl.x;
    float dy = point.y - cyl.y;
    float distanceSquared = dx * dx + dy * dy;
    if (distanceSquared >= cyl.radius * cyl.radius)
        return false;
    return true;
}

bool CollisionManager::collisionCylinderBoundary(const Collider& cyl) {
    if (cyl.x-cyl.radius < 0 || cyl.x+cyl.radius > BOUNDARY_LEN || cyl.y-cyl.radius < 0 || cyl.y+cyl.radius > BOUNDARY_LEN) 
        return true;
    return false;
}

bool CollisionManager::checkCollisionCylinder(const Collider& cyl) {
    if (collisionCylinderBoundary(cyl))
        return true;
    for (size_t i = 0; i < colliders.size(); i++) {
        if (colliders[i]->owner != cyl.owner) {
            if ((!colliders[i]->isSector) && (!colliders[i]->isPoint) 
                && collisionCylinderCylinder(cyl, *(colliders[i].get()))) 
                return true;
        }
    }
    return false;
}

