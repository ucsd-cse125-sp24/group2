#include "EnemyComponent.hpp"
#include "AnimationPlayer.h"

void EnemyComponent::Update(float deltaTime) {
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();

    // switch attack animations
    switch (atk) {
        case ((int) AttackState::SWIPE): {
            if (animationDuration <= 0) {
                owner->GetComponent<AnimationPlayer>()->play("attack", true);
                animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("attack")->getDuration() / 1000.0f;
            }
            break;
        }
        case ((int) AttackState::LASER): {
            if (animationDuration <= 0) {
                owner->GetComponent<AnimationPlayer>()->play("attack2", true);
                animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("attack2")->getDuration() / 1000.0f;
            }
            break;
        }
        case ((int) AttackState::MARK): {
            if (animationDuration <= 0) {
                owner->GetComponent<AnimationPlayer>()->play("spell", true);
                animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("spell")->getDuration() / 1000.0f;
                
            }
            break;
        }
        case ((int) AttackState::STOMP): {
            if (animationDuration <= 0) {
                owner->GetComponent<AnimationPlayer>()->play("attack3", true);
                animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("attack3")->getDuration() / 1000.0f;
            }
            break;
        }
        case ((int) AttackState::DEAD): {
            owner->GetComponent<AnimationPlayer>()->play("dying", false);
            if (deadAnimation <= 0) {
                owner->GetComponent<AnimationPlayer>()->play("dead", false);
            } else {
                deadAnimation -= deltaTime;
            }
            break;
        }
        case ((int) AttackState::IDLE): {
            if (animationDuration <= 0) {
                animationPlayer->play("idle", false);
            }
            break;
        }
        default: {
            break;
        }
    }

    animationDuration = std::max(0.0f, animationDuration - deltaTime);
}