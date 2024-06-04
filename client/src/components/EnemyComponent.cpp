#include "EnemyComponent.hpp"
#include "AnimationPlayer.h"

void EnemyComponent::Update(float deltaTime) {
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();
    // TODO:
    // switch attack animations
    switch (atk) {
        case ((int) AttackState::SWIPE): {
            // std::cout << "playing swipe" << std::endl;
            break;
        }
        case ((int) AttackState::LASER): {
            // std::cout << "playing laser" << std::endl;
            break;
        }
        case ((int) AttackState::MARK): {
            // std::cout << "playing mark" << std::endl;
            break;
        }
        case ((int) AttackState::STOMP): {
            // std::cout << "playing stomp" << std::endl;
            break;
        }
        default: {
            // No attack just idle
            animationPlayer->play("idle");
            break;
        }
    }
}