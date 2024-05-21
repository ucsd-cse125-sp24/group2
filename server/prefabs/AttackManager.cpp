#include "CollisionManager.hpp"
#include "AttackManager.hpp"
#include <iostream>
#include "Health.hpp"

void AttackManager::newPlayerAttack(Player* p) {
    std::lock_guard<std::mutex> lock(_attack_mutex);
    PlayerAttack* playerAtt = new PlayerAttack();
    playerAtt->init(p);
    playerAtt->setTarget(enemyPrefab);
    playerAttackList.push_back(playerAtt);
}

void AttackManager::addPlayer(Player* p) {
    std::lock_guard<std::mutex> lock(_player_mutex);
    // Initialize and Register Player Collider in CollisionManager
    Collider* c = new Collider(p, p->GetComponent<Transform>());
    c->SetRadius(10);
    c->SetHeight(10);
    c->SetRotation(glm::vec3(1, 0, 0));
    p->AddComponent(c);
    CollisionManager::instance().add(p);
    playerList.push_back(p);
}

void AttackManager::addEnemy(Enemy* e) {
    enemyPrefab = e;
    // Initialize and Register Enemy Collider in CollisionManager
    Collider* c = new Collider(enemyPrefab, enemyPrefab->GetComponent<Transform>());
    // for testing purposes
    c->SetRadius(20);
    c->SetHeight(50);
    // end testing purposes
    enemyPrefab->AddComponent(c);
    CollisionManager::instance().add(enemyPrefab);
    Health* h = new Health(enemyPrefab, 100);
    enemyPrefab->AddComponent(h);
}

void AttackManager::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(_attack_mutex);
    for(int i = playerAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside loop
        if(!playerAttackList.at(i)->isExist()) {
            playerAttackList.erase( playerAttackList.begin() + i );
            continue;
        }
        playerAttackList.at(i)->update(deltaTime);
    }
}