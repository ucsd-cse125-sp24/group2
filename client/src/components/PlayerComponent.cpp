#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>

void PlayerComponent::Update(float deltaTime) {
    if (glm::length(owner->GetComponent<Mover>()->movementHeading) > 0) {
        owner->GetComponent<AnimationPlayer>()->play("running");
    } else {
        owner->GetComponent<AnimationPlayer>()->play("idle");
    }
}