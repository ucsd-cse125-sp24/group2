#include "CollisionManager.hpp"
#include "AttackManager.hpp"
#include <iostream>
#include "Health.hpp"
#include "LaserAttack.hpp"
#include "MarkedAttack.hpp"
#include "StompAttack.hpp"
#include "SwipeAttack.hpp"
#include "NetworkManager.hpp"
#include "EnemyComponent.hpp"
#include "Heal.hpp"
#include "SpeedUp.hpp"
#include "Revive.hpp"

void AttackManager::newPlayerAttack(Player* p) {
    std::lock_guard<std::mutex> lock(_player_attack_mutex);
    PlayerAttack* playerAtt = new PlayerAttack();
    playerAtt->init(p);
    playerAtt->setTarget(enemyPrefab);
    playerAttackList.push_back(playerAtt);
    NetworkManager::instance().scene.Instantiate(playerAtt);
}

void AttackManager::newPlayerHeal(Player* p) {
    Heal* playerHeal = new Heal(p);
    playerSkillList.push_back(playerHeal);
    NetworkManager::instance().scene.Instantiate(playerHeal);
}

void AttackManager::newPlayerRevive(Player* p) {
    Revive* playerRevive = new Revive(p);
    playerSkillList.push_back(playerRevive);
    NetworkManager::instance().scene.Instantiate(playerRevive);
}

void AttackManager::newPlayerSpeedUp(Player* p) {
    SpeedUp* playerSpeedUp = new SpeedUp(p);
    playerSkillList.push_back(playerSpeedUp);
    NetworkManager::instance().scene.Instantiate(playerSpeedUp);
}

void AttackManager::addPlayer(Player* p) {
    std::lock_guard<std::mutex> lock(_player_mutex);
    playerList.push_back(p);
}

void AttackManager::addEnemy(Enemy* e) { enemyPrefab = e; }

void AttackManager::newLaserAttack() {
    LaserAttack* laserFront = new LaserAttack(enemyPrefab, 0);
    enemyAttackList.push_back(laserFront);
    NetworkManager::instance().scene.Instantiate(laserFront);
    LaserAttack* laserLeft = new LaserAttack(enemyPrefab, 1);
    enemyAttackList.push_back(laserLeft);
    NetworkManager::instance().scene.Instantiate(laserLeft);
    LaserAttack* laserBack = new LaserAttack(enemyPrefab, 2);
    enemyAttackList.push_back(laserBack);
    NetworkManager::instance().scene.Instantiate(laserBack);
    LaserAttack* laserRight = new LaserAttack(enemyPrefab, 3);
    enemyAttackList.push_back(laserRight);
    NetworkManager::instance().scene.Instantiate(laserRight);
    // J: this functionality could be placed in the attack initializations
    // idk this seems a bit more centralized and is less work
    enemyPrefab->GetComponent<EnemyComponent>()->SetState(AttackState::LASER);
}

void AttackManager::newStompAttack() {
    StompAttack* stompAtt = new StompAttack(enemyPrefab);
    enemyAttackList.push_back(stompAtt);
}

void AttackManager::newMarkedAttack() {
    for (int i = 0; i < playerList.size(); i++) {
        MarkedAttack* markedAtt = new MarkedAttack(enemyPrefab, playerList[i]);
        enemyAttackList.push_back(markedAtt);
        NetworkManager::instance().scene.Instantiate(markedAtt);
        markedAtt->GetComponent<NetTransform>()->position =
            playerList[i]->GetComponent<NetTransform>()->position;
    }
    enemyPrefab->GetComponent<EnemyComponent>()->SetState(AttackState::MARK);
}

void AttackManager::newSwipeAttack() {
    SwipeAttack* swipeAtt = new SwipeAttack(enemyPrefab);
    enemyAttackList.push_back(swipeAtt);
    enemyPrefab->GetComponent<EnemyComponent>()->SetState(AttackState::SWIPE);
    NetworkManager::instance().scene.Instantiate(swipeAtt);
}

void AttackManager::update(float deltaTime) {
    // J: this is such a wacky way to keep track of number of players alive rn
    // okay we are also going to use this to set player's centers uhhhh
    // we should really probably just keep a list of players in networkmanager
    int numAlive = 0;
    for (Player* player : playerList) {
        if (player->GetComponent<Health>()->GetHealth() > 0) {
            numAlive++;
        }
        if (enemyPrefab) {
            glm::vec3 enemyPosition =
                enemyPrefab->GetComponent<NetTransform>()->GetPosition();
            // J: i guess make sure to set the y to 0
            player->GetComponent<Mover>()->SetCenter(enemyPosition);
        }
    }
    NetworkManager::instance().numAlive = numAlive;

    std::lock_guard<std::mutex> playerlock(_player_attack_mutex);
    for (int i = playerAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside
        // loop
        if (!playerAttackList.at(i)->isExist()) {
            NetworkManager::instance().scene.Destroy(playerAttackList.at(i));
            playerAttackList.erase(playerAttackList.begin() + i);
            continue;
        }
    }

    for (int i = playerSkillList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside
        // loop
        if (!playerSkillList.at(i)->isExist()) {
            NetworkManager::instance().scene.Destroy(playerSkillList.at(i));
            playerSkillList.erase(playerSkillList.begin() + i);
            continue;
        }
    }

    for (int i = enemyAttackList.size() - 1; i >= 0; i--) {
        // iterate from the back to take care of the situ of removing inside
        // loop
        if (!enemyAttackList.at(i)->exist) {
            NetworkManager::instance().scene.Destroy(enemyAttackList.at(i));
            enemyAttackList.erase(enemyAttackList.begin() + i);
            continue;
        }
    }
}