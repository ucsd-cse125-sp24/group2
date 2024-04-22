#include "../include/TestEntity.hpp"

TestEntity::TestEntity() : GameObject() {
    components[0]->subscribe(testPosition);
    components[0]->subscribe(testRotation);
    components[0]->subscribe(testScale);
}