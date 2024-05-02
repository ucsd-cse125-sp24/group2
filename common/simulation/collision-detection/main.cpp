#include <iostream>
#include "CollisionManager.h"  // Make sure this header includes all necessary Collider definitions and methods
#include <cmath>

# define PI           3.14159265358979323846

int main() {
    CollisionManager manager;
    std::cout << std::boolalpha;
    
    // Add some colliders
    // addCylinder(int owner, float x, float y, float z, float radius, float height)
    Collider* cyl1 = manager.addCylinder(2, 10, 10, 0, 4, 10);
    // Collider* cyl2 = manager.addCylinder(1, 15, 15, 0, 3, 6);
    // std::cout << manager.collisionCylinderCylinder(*cyl1, *cyl2) << std::endl;
    // manager.moveCylinder(cyl1, 11, 11, 0, 4, 10);
    // std::cout << cyl1->x << " " << cyl1->y << std::endl;
    // std::cout << manager.collisionCylinderCylinder(*cyl1, *cyl2) << std::endl;
    // manager.moveCylinder(cyl1, 9, 9, -1, 4, 10);
    // std::cout << cyl1->x << " " << cyl1->y << " " << cyl1->z << std::endl;
    // std::cout << manager.collisionCylinderCylinder(*cyl1, *cyl2) << std::endl;
    // manager.moveCylinder(cyl1, 496, 496, -1, 4, 10);
    // manager.moveCylinder(cyl1, 499, 499, -1, 4, 10);
    // std::cout << cyl1->x << " " << cyl1->y << " " << cyl1->z << std::endl;
    // std::cout << manager.collisionCylinderCylinder(*cyl1, *cyl2) << std::endl;
    // manager.moveCylinder(cyl1, 1, 1, -1, 4, 10);
    // std::cout << cyl1->x << " " << cyl1->y << " " << cyl1->z << std::endl;
    // std::cout << manager.collisionCylinderCylinder(*cyl1, *cyl2) << std::endl;


    std::cout << cos(PI) << std::endl;
    Collider* sec1 = manager.addSector(2, 14.1, 10, 0, 4, 10, 0, PI/2);
    std::cout << manager.collisionCylinderSector(*cyl1, *sec1) << std::endl;
    Collider* sec2 = manager.addSector(2, 14.1, 10, 0, 4, 10, 0, PI/2+1);
    std::cout << manager.collisionCylinderSector(*cyl1, *sec2) << std::endl;

    Collider* sec3 = manager.addSector(2, 15.65685425, 10, 0, 4, 10, PI/2, PI*3/4); // 5.6... a little more than 4sqrt(2). 
    std::cout << manager.collisionCylinderSector(*cyl1, *sec3) << std::endl; // Should be false 
    manager.moveSector(sec3, 15.65685425, 10, 0, 4, 10, PI*3/4, PI*3/4+PI*1/10); 
    std::cout << manager.collisionCylinderSector(*cyl1, *sec3) << std::endl; // Should be true 

    Collider* pt1 = manager.addPoint(2, 13.9, 10, 3);
    std::cout << manager.collisionCylinderPoint(*cyl1, *pt1) << std::endl;

    // Assume you have some mechanism to properly close out or manage memory
    return 0;
}
