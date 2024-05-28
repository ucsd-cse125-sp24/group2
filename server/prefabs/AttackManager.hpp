#ifndef ATTACKMANAGER_HPP
#define ATTACKMANAGER_HPP

#include <vector>
#include <mutex>

#include "PlayerAttack.hpp"
#include "Enemy.hpp"
#include "EnemyAttack.hpp"

class AttackManager {
    private:
    std::mutex _player_attack_mutex;
    std::mutex _player_mutex;

    public:
    std::vector<PlayerAttack*> playerAttackList;
    std::vector<EnemyAttack*> enemyAttackList;
    std::vector<Player*> playerList;
    Enemy* enemyPrefab;
    
    void addEnemy(Enemy* e);
    void addPlayer(Player* p);
    void newPlayerAttack(Player* p);
    void newLaserAttack();
    void newStompAttack();
    void newMarkedAttack();
    void newSwipeAttack();
    void update(float deltaTime);

    static AttackManager& instance() {
        static AttackManager s;
        return s;
    }

};

#endif // ATTACKMANAGER_HPP