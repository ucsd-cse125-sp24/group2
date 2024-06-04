#include "PlayerSkillType.hpp"
#include "AnimationPlayer.h"

void PlayerSkillType::Update(float deltaTime) {
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();
    // TODO:
    // switch attack animations
    switch (type) {
        case ((int) SkillType::PLAYER_ATTACK): {
            std::cout << "playing swipe" << std::endl;
            break;
        }
        case ((int) SkillType::HEAL): {
            std::cout << "playing laser" << std::endl;
            break;
        }
        default: {
            // No attack just idle
            animationPlayer->play("idle");
            break;
        }
    }
}