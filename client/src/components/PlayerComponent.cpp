#include "components/PlayerComponent.hpp"
#include <InputManager.h>
#include <AnimationPlayer.h>
#include <Mover.hpp>
#include <NetTransform.hpp>

void PlayerComponent::Update(float deltaTime) {
    if (glm::length(owner->GetComponent<Mover>()->movementHeading) > 0) {
        std::cout << "player "
                  << (dynamic_cast<NetworkObject*>(owner))->networkId()
                  << " is moving" << std::endl;
        owner->GetComponent<AnimationPlayer>()->play("run");
    } else {
        owner->GetComponent<AnimationPlayer>()->play("idle");
    }
}