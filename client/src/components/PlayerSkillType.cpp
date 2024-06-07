#include "PlayerSkillType.hpp"
#include "AnimationPlayer.h"

void PlayerSkillType::Update(float deltaTime) {
    AnimationPlayer* animationPlayer = owner->GetComponent<AnimationPlayer>();
    // TODO:
    // switch attack animations
    switch (type) {
        case ((int) SkillType::PLAYER_ATTACK): {
            // std::cout << "playing player_attack" << std::endl;
            break;
        }
        case ((int) SkillType::HEAL): {
            // std::cout << "playing heal" << std::endl;
            break;
        }
        case ((int) SkillType::REVIVE): {
            // std::cout << "playing revive" << std::endl;
            break;
        }
        case ((int) SkillType::SPEED_BOOST): {
            // std::cout << "playing speed_boost" << std::endl;
            break;
        }
        default: {
            // No attack just idle
            std::cout << "default case in PlayerSkillType Update" << std::endl;
            // animationPlayer->play("idle");
            break;
        }
    }
}