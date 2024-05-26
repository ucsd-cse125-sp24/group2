#include "EnemyAttack.hpp"
#include "NetTransform.hpp"
#include "Health.hpp"
#include "CollisionManager.hpp"
#include <iostream>

# define PI           3.14159265358979323846

std::vector<double> right_angles = {0, 0.5 * PI, PI, 1.5 * PI};

EnemyAttack::EnemyAttack() : Entity() { exist = true; }

EnemyAttack::EnemyAttack(int networkId) : Entity(networkId) { exist = true; }


// attack_id: 
// 1. jump and stomp, shockwave expanding 
// 2. mark players and cause damage
// 3. 4 lasers swiping
// 4. normal attack: swipe hit
// void EnemyAttack::init(Enemy* owner, int attack_id, GameObject* target) {
//     switch (attack_id) {
//         case 1:
//             attackC = new Collider(this, owner->GetComponent<Collider>());
//             break;
//         case 2:
//             attackC = new Collider(this, target->GetComponent<Collider>());
//             break;
//         case 3:
//             attackC = new Collider(this, owner->GetComponent<Collider>());
//             attackC->makeSector(0.01);
//             attackC->SetStartAngle(attackC->GetStartAngle() + right_angles[count_right_angle]);
//             attackC->SetEndAngle(attackC->GetEndAngle() + right_angles[count_right_angle++]);
//             attackC->SetHeight(3);
//             attackC->SetRadius(500);
//             break;
//         case 4:
//             attackC = new Collider(this, owner->GetComponent<Collider>());
//     }
    
    
//     auto newY = attackC->GetPosition().y + attackC->GetHeight()/2;
//     glm::vec3 rotationXZ(attackC->GetRotation().x, 0, attackC->GetRotation().z);
//     glm::vec3 normRotationXZ = glm::normalize(rotationXZ);
//     auto newX = attackC->GetPosition().x + normRotationXZ.x * attackC->GetRadius();
//     auto newZ = attackC->GetPosition().z + normRotationXZ.z * attackC->GetRadius();
//     glm::vec3 newPosition(newX, newY, newZ);
//     attackC->SetPosition(newPosition);
//     attackC->makePoint();
//     this->AddComponent(attackC);
//     this->GetComponent<NetTransform>()->SetPosition(newPosition);
// }


void EnemyAttack::DealDamage(std::vector<GameObject*> players_hit) {
    if (exist) {
        for (GameObject* player : players_hit) {
            player->GetComponent<Health>()->ChangeHealth(-damage);
        }
        exist = false;
        CollisionManager::instance().remove(this);
    }
}