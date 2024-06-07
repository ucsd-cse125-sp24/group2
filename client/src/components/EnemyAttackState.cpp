#include "EnemyAttackState.hpp"
#include "AnimationPlayer.hpp"

void EnemyAttackState::Update(float deltaTime) {
    switch (atk) {
        case (int)AttackState::SWIPE:
            owner->GetComponent<AnimationPlayer>()->play("swipe-animation");
            break;
        case (int)AttackState::LASER:
            owner->GetComponent<AnimationPlayer>()->play("laser-spin", false);
            break;
        case (int)AttackState::MARK:
            owner->GetComponent<AnimationPlayer>()->play("projectile-bone-anim", false);
            break;
        case (int)AttackState::STOMP:
            owner->GetComponent<AnimationPlayer>()->play("waveAttack", false);
            break;
    }
}