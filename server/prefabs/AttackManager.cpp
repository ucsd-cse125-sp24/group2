#include "CollisionManager.hpp"
#include "AttackManager.hpp"
#include <iostream>
#include "Health.hpp"
#include "LaserAttack.hpp"

void AttackManager::newPlayerAttack(Player* p) {
    std::lock_guard<std::mutex> lock(_player_attack_mutex);
    PlayerAttack* playerAtt = new PlayerAttack();
    playerAtt->init(p);
    playerAtt->setTarget(enemyPrefab);
    playerAttackList.push_back(playerAtt);
}

void AttackManager::addPlayer(Player* p) {
    std::lock_guard<std::mutex> lock(_player_mutex);
    // Initialize and Register Player Collider in CollisionManager
    Collider* c = new Collider(p, p->GetComponent<NetTransform>());
    c->SetRadius(10);
    c->SetHeight(10);
    c->SetRotation(glm::vec3(1, 0, 0));
    p->AddComponent(c);
    CollisionManager::instance().add(p);
    playerList.push_back(p);
}

void AttackManager::addEnemy(Enemy* e) {
    enemyPrefab = e;
}

void AttackManager::newLaserAttack() {
    LaserAttack* laserAtt = new LaserAttack(enemyPrefab);
    enemyAttackList.push_back(laserAtt);
}

// TODO
// void AttackManager::newStompAttack() {
//     StompAttack* stompAtt = new StompAttack();
//     enemyAttackList.push_back(stompAtt);
// }

// TODO
// void AttackManager::newMarkedAttack() {
//     MarkedAttack* markedAtt = new MarkedAttack();
//     enemyAttackList.push_back(markedAtt);
// }

// TODO
// void AttackManager::newSwipeAttack() {
//     SwipeAttack* swipeAtt = new SwipeAttack();
//     enemyAttackList.push_back(swipeAtt);
// }

void AttackManager::update(float deltaTime) {
    std::lock_guard<std::mutex> playerlock(_player_attack_mutex);
    for(int i = playerAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside loop
        if(!playerAttackList.at(i)->isExist()) {
            playerAttackList.erase( playerAttackList.begin() + i );
            continue;
        }
        playerAttackList.at(i)->update(deltaTime);
    }

    for(int i = enemyAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside loop
        if(!enemyAttackList.at(i)->exist) {
            enemyAttackList.erase( enemyAttackList.begin() + i );
            continue;
        }
        enemyAttackList.at(i)->update(deltaTime);
    }
}