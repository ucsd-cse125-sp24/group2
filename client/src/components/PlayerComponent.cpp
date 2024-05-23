#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>

void PlayerComponent::Update(float deltaTime) {
    if (glm::length(owner->GetComponent<Mover>()->movementHeading) > 0) {
        owner->GetComponent<AnimationPlayer>()->play("run");
        std::cout << owner->GetComponent<AnimationPlayer>()->currentAnimation
                  << std::endl;
    } else {
        owner->GetComponent<AnimationPlayer>()->play("idle");
    }
}