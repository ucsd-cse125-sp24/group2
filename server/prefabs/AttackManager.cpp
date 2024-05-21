#include "CollisionManager.hpp"
#include "AttackManager.hpp"


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
    Collider* c = new Collider(p, p->GetComponent<NetTransform>());
    p->AddComponent(c);
    CollisionManager::instance().add(p);
    playerList.push_back(p);
}

void AttackManager::addEnemy(Enemy* e) {
    enemyPrefab = e;
    // Initialize and Register Enemy Collider in CollisionManager
    Collider* c = new Collider(enemyPrefab, enemyPrefab->GetComponent<NetTransform>());
    enemyPrefab->AddComponent(c);
    CollisionManager::instance().add(enemyPrefab);
}

void AttackManager::update(float deltaTime) {
    std::lock_guard<std::mutex> lock(_attack_mutex);
    for(int i = playerAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside loop
        if(!playerAttackList.at(i)->isExist()) {
            playerAttackList.erase( playerAttackList.begin() + i ); 
        }
        playerAttackList.at(i)->update(deltaTime);
    }
}