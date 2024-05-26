#ifndef COLLISIONMANAGER_HPP
#define COLLISIONMANAGER_HPP

#include <vector>
#include <algorithm>
#include <array>
#include <cmath>
#include <unordered_map>
#include <mutex>

#include "Collider.hpp"
#include "GameObject.hpp"
#include "Transform.hpp"

#define BOUNDARY_LEN 500

// Cylinder is used for objects
// CylindricalSector is used for attack range
// Point is for music (magic) attack
// Therefore, the only collsions possible are Cylinder&Cylinder,
// Cylinder&Sector, Cylinder&Point, Cylinder&Floor/Boundary

class CollisionManager {
private:
    std::mutex
        _mutex; // everything touching colliderOwners should be mutex protected
    std::unordered_map<Collider*, GameObject*> colliderOwners;

public:
    bool add(GameObject* owner);

    void remove(GameObject* owner);

    bool move(GameObject* owner, glm::vec3 newPosition);
    bool move(GameObject* owner, glm::vec3 newPosition, glm::vec3 newRotation,
              glm::vec3 newScale);

    // These may not need to be public in the future
    bool collisionCylinderCylinder(const Collider* cyl1, const Collider* cyl2);
    bool collisionCylinderSector(const Collider* cyl, const Collider* sec);
    bool collisionCylinderPoint(const Collider* cyl, const Collider* point);
    bool collisionCylinderBoundary(const Collider* cyl);
    bool checkCollisionCylinder(Collider* cyl);
    bool checkCollisionAttack(Collider* att);
    static CollisionManager& instance() {
        static CollisionManager s;
        return s;
    }
};

struct Vector2 {
    double x, y;

    // Normalize the vector
    Vector2 normalize() const {
        double len = std::sqrt(x * x + y * y);
        return {x / len, y / len};
    }

    // Dot product of two vectors
    double dot(const Vector2& other) const { return x * other.x + y * other.y; }

    // Cross product of two vectors
    double cross(const Vector2& other) const {
        return x * other.y - y * other.x;
    }
};

static bool isBetween(const Vector2& C, const Vector2& A, const Vector2& B) {
    // Normalize vectors
    Vector2 nA = A.normalize();
    Vector2 nB = B.normalize();
    Vector2 nC = C.normalize();

    // Cross products to check the plane and direction
    double crossAC = nA.cross(nC);
    double crossAB = nA.cross(nB);
    double crossBC = nB.cross(nC);
    double crossBA = nB.cross(nA);

    // Check if the directions of the cross products are consistent
    bool cond1 = crossAC * crossAB > 0; // C is on the same side of A as B
    bool cond2 = crossBC * crossBA > 0; // C is on the same side of B as A

    return cond1 && cond2;
}

#endif // COLLISIONMANAGER_HPP
