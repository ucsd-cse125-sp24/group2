#include <vector>
#include <memory>  // For std::unique_ptr
#include "Collider.h"
#include <algorithm>

#define BOUNDARY_LEN 500

// Cylinder is used for objects
// CylindricalSector is used for attack range
// Point is for music (magic) attack
// Therefore, the only collsions possible are Cylinder&Cylinder, Cylinder&Sector, Cylinder&Point, Cylinder&Floor/Boundary

class CollisionManager {
    std::vector<std::unique_ptr<Collider>> colliders;
    int countOwner = 0;

public:
    Collider* addCylinder(int owner, float x, float y, float z, float radius, float height);
    Collider* addSector(int owner, float x, float y, float z, float radius, float height, float startAngle, float endAngle);
    Collider* addPoint(int owner, float x, float y, float z);

    void removeCollider(Collider* collider);

    void moveCylinder(Collider* collider, float nx, float ny, float nz, float nradius, float nheight);
    void moveSector(Collider* collider, float nx, float ny, float nz, float nradius, float nheight, float nstartAngle, float nendAngle);
    void movePoint(Collider* collider, float nx, float ny, float nz);

    // These may not need to be public in the future
    bool collisionCylinderCylinder(const Collider& cyl1, const Collider& cyl2);
    bool collisionCylinderSector(const Collider& cyl, const Collider& sec);
    bool collisionCylinderPoint(const Collider& cyl, const Collider& point);
    bool collisionCylinderBoundary(const Collider& cyl);
    bool checkCollisionCylinder(const Collider& cyl);
};