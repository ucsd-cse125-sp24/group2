#include "CollisionManager.hpp"
#include "AttackManager.hpp"
#include <iostream>
#include "Health.hpp"
#include "LaserAttack.hpp"
#include "MarkedAttack.hpp"
#include "SwipeAttack.hpp"
#include "NetworkManager.hpp"
#include "EnemyComponent.hpp"

void AttackManager::newPlayerAttack(Player* p) {
    std::lock_guard<std::mutex> lock(_player_attack_mutex);
    PlayerAttack* playerAtt = new PlayerAttack();
    playerAtt->init(p);
    playerAtt->setTarget(enemyPrefab);
    playerAttackList.push_back(playerAtt);
    NetworkManager::instance().scene.Instantiate(playerAtt);
}

void AttackManager::addPlayer(Player* p) {
    std::lock_guard<std::mutex> lock(_player_mutex);
    playerList.push_back(p);
}

void AttackManager::addEnemy(Enemy* e) {
    enemyPrefab = e;
}

void AttackManager::newLaserAttack() {
    LaserAttack* laserAtt = new LaserAttack(enemyPrefab);
    enemyAttackList.push_back(laserAtt);
    NetworkManager::instance().scene.Instantiate(laserAtt);
    laserAtt->GetComponent<EnemyComponent>()->SetState(AttackState::LASER);
}

// TODO
// void AttackManager::newStompAttack() {
//     StompAttack* stompAtt = new StompAttack();
//     enemyAttackList.push_back(stompAtt);
//     NetworkManager::instance().scene.Instantiate(stompAtt);
// }

void AttackManager::newMarkedAttack() {
    MarkedAttack* markedAtt = new MarkedAttack(enemyPrefab, playerList);
    enemyAttackList.push_back(markedAtt);
    NetworkManager::instance().scene.Instantiate(markedAtt);
    markedAtt->GetComponent<EnemyComponent>()->SetState(AttackState::MARK);
}

void AttackManager::newSwipeAttack() {
    SwipeAttack* swipeAtt = new SwipeAttack(enemyPrefab);
    enemyAttackList.push_back(swipeAtt);
    NetworkManager::instance().scene.Instantiate(swipeAtt);
    swipeAtt->GetComponent<EnemyComponent>()->SetState(AttackState::SWIPE);
}

void AttackManager::update(float deltaTime) {
    // this is such a wacky way to keep track of number of players alive rn
    int numAlive = 0;
    for (Player* player : playerList) {
        if (player->GetComponent<Health>()->GetHealth() > 0) {
            numAlive++;
        }
    }
    NetworkManager::instance().numAlive = numAlive;

    std::lock_guard<std::mutex> playerlock(_player_attack_mutex);
    for(int i = playerAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside loop
        if(!playerAttackList.at(i)->isExist()) {
            playerAttackList.erase( playerAttackList.begin() + i );
            NetworkManager::instance().scene.Destroy(playerAttackList.at(i));
            continue;
        }
    }

    for(int i = enemyAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside loop
        if(!enemyAttackList.at(i)->exist) {
            enemyAttackList.erase( enemyAttackList.begin() + i );
            NetworkManager::instance().scene.Destroy(enemyAttackList.at(i));
            continue;
        }
    }
}