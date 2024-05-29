#include "EnemyAttack.hpp"
#include "NetTransform.hpp"
#include "Health.hpp"
#include "Player.hpp"

#include <iostream>

EnemyAttack::EnemyAttack(Enemy* owner) : Entity() { 
    enemy = owner;
    exist = true;
}

EnemyAttack::EnemyAttack(Enemy* owner, int networkId) : Entity(networkId) { 
    enemy = owner;
    exist = true;
}


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


void EnemyAttack::DealDamage(std::vector<GameObject*> entity_hit) {
    if (exist) {
        for (GameObject* entity : entity_hit) {
            if (entity != enemy) {
                entity->GetComponent<Health>()->ChangeHealth(-damage);
                //std::cout << "New player health: " << entity->GetComponent<Health>()->GetHealth() << std::endl;

                // Check for death
                if(entity->GetComponent<Health>()->GetHealth() <= 0){
                    dynamic_cast<Player*>(entity)->alive = false;
                }
            }
        }
    }
}