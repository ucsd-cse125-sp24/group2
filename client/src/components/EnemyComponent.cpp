#include "EnemyComponent.hpp"
#include "AnimationPlayer.h"

void EnemyComponent::Update(float deltaTime) {
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();

    // switch attack animations
    switch (atk) {
        case ((int) AttackState::SWIPE): {
            owner->GetComponent<AnimationPlayer>()->play("attack");
            animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("attack")->getDuration() / 1000.0f;
            break;
        }
        case ((int) AttackState::LASER): {
            owner->GetComponent<AnimationPlayer>()->play("attack2");
            animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("attack2")->getDuration() / 1000.0f;
            break;
        }
        case ((int) AttackState::MARK): {
            owner->GetComponent<AnimationPlayer>()->play("spell");
            animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("spell")->getDuration() / 1000.0f;
            break;
        }
        case ((int) AttackState::STOMP): {
            // owner->GetComponent<AnimationPlayer>()->play("");
            // animationDuration = owner->GetComponent<AnimationPlayer>()->GetClip("attack2")->getDuration() / 1000.0f;
            break;
        }
        case ((int) AttackState::IDLE): {
            if (animationDuration <= 0) {
                animationPlayer->play("idle");
            } else {
                animationDuration -= deltaTime;
            }
            break;
        }
        default: {
            break;
        }
    }
}