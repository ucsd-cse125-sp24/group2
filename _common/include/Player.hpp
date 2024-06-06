#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#define GLEW_STATIC
#include "../../client/include/GL/glew.h"
#include "../../client/include/GLFW/glfw3.h"
// #include "../../client/include/
// #include "../../client/include/core.h"

#include "Entity.hpp"
#include "Transform.hpp"
#include "NetTransform.hpp"
#include "Mover.hpp"

enum Keys {
    I = 73,
    J = 74,
    K = 75,
    L = 76
};

class Player : public Entity {
public:
    // glm::vec2 inputs = glm::vec2();
    // glm::vec3 velocity = glm::vec3();
    // float speed = 0.2f;
    bool alive;

    // I = 73, J = 74, K = 75, L = 76
    std::vector<int> attack1 = {J, J, J, J};
    std::vector<int> attack2 = {J, K, L, I};
    std::vector<int> heal = {K, K, K, K};
    std::vector<int> revive = {L, L, L, L};
    std::vector<int> speedBoost = {I, I, I, I};

    int32_t TypeID() const override { return PLAYER; }

    // Used by server
    Player(glm::vec3 position);
    void onDestroy();

    virtual void update(float deltaTime) override;

    // Used by client
    Player(std::string path, int networkId);
};