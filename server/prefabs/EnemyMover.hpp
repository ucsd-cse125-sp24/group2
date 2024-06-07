#pragma once


#include "IComponent.hpp"
#include "NetworkManager.hpp"


class EnemyMover : public IComponent {
    public:
        Player* targetPlayer = nullptr;

        float baseSpeed = 0.66f;
        glm::vec3& position;
        glm::vec3& rotation;

        EnemyMover(GameObject* owner);

        void Update(float deltaTime) override;
        std::string ToString() override;
};