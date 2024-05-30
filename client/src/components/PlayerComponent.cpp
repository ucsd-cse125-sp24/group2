#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>
#include <Health.hpp>

void PlayerComponent::Update(float deltaTime) {
    if (owner->GetComponent<Health>()->GetHealth() <= 0) {
        owner->GetComponent<NetTransform>()->SetRotation(glm::vec3(90.0f, 0.0f, 0.0f));
        //owner->GetComponent<AnimationPlayer>()->play("dead");
        owner->GetComponent<AnimationPlayer>()->play("idle");
    } else if (glm::length(owner->GetComponent<Mover>()->movementHeading) > 0) {
        owner->GetComponent<AnimationPlayer>()->play("run");
    } else {
        owner->GetComponent<AnimationPlayer>()->play("idle");
    }
}