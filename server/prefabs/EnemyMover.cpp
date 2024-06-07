#include "EnemyMover.hpp"
#include "SkillTraits.hpp"
#include "Client.hpp"
#include "Enemy.hpp"
#include "CollisionManager.hpp"
#include "Health.hpp"
#include <limits>


EnemyMover::EnemyMover(GameObject* owner) : 
    IComponent(owner),
    position(owner->GetComponent<NetTransform>()->position),
    rotation(owner->GetComponent<NetTransform>()->rotation)
{}

/**
 * Boss moves towards closest player
*/
void EnemyMover::Update(float deltaTime) {
    Enemy::EnemyState currEnemyState = static_cast<Enemy*>(owner)->GetEnemyState();
    if (currEnemyState == Enemy::PHASE5) {
        return;
    }

    std::vector<Client*> clients = NetworkManager::instance().get_clients();

    if (clients.size() == 0) {
        // std::cout << "ERROR: no clients in EnemyMover::Update(float deltaTime)" << std::endl;
        return;
    }

    float minDistance = std::numeric_limits<float>::infinity();
    for (int i = 0; i < clients.size(); ++i) {
        // only target alive players
        if (clients[i]->p->GetComponent<Health>()->GetDead()) {
            continue;
        }
        Player* currPlayer = clients[i]->p;
        float currDistance = glm::distance(currPlayer->GetComponent<NetTransform>()->GetPosition(), position);
        if (currDistance < minDistance) {
            targetPlayer = currPlayer;
            minDistance = currDistance;
        }
    }

    glm::vec3 closestPlayerPosition = targetPlayer->GetComponent<NetTransform>()->GetPosition();
    glm::vec3 bossHeading = glm::normalize(closestPlayerPosition - position);
    rotation.y = 90.0f + glm::degrees(glm::acos(glm::dot(bossHeading, glm::vec3(0,0,-1))));
    if (bossHeading.x > 0) rotation.y = -rotation.y + 180.0f;
    Collider* collider = owner->GetComponent<Collider>();
    collider->SetRotation(rotation);

    bool isChasedown = currEnemyState == Enemy::PHASE1; // NOTE: can include any phase u want with a cheeky lil ||
    if (isChasedown) {
        if (minDistance - baseSpeed > (float)SW_RADIUS - 3.0f) { // stop when player is 3 m deep in your attack range
            glm::vec3 newPosition = position + bossHeading * baseSpeed;
            CollisionManager::instance().move(owner, newPosition);
        } 
    } else if (glm::length(glm::vec3() - position) > baseSpeed) {
        position += glm::normalize(glm::vec3() - position) * baseSpeed;
    } else {
        position = glm::vec3();
        collider->SetPosition(glm::vec3());
        if (CollisionManager::instance().checkCollisionCylinder(collider)) {
            if (transparent) {
                CollisionManager::instance().remove(owner);
            }
        }
        else {
            if (transparent) {
                CollisionManager::instance().add(owner);
                transparent = false;
            }
        }
    }
}

std::string EnemyMover::ToString() { return "EnemyMover"; }