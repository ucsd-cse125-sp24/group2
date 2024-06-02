#include "Player.hpp"

#include "Mover.hpp"
#include "components/Model.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "InputManager.h"
#include "components/RendererComponent.hpp"

Player::Player(std::string path, int networkId) : Entity(networkId) {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    MovementStateMachine* movementStateMachine = new MovementStateMachine(this);
    AddComponent(movementStateMachine);
    RendererComponent* meshRenderer =
        new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(meshRenderer);
    Model* model = new Model(AssetManager::Instance().GetModel(path));
    AddComponent(model);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
}

void Player::update(float deltaTime) {
    if ((InputManager::IsKeyDown(GLFW_KEY_W) ||
         InputManager::IsKeyDown(GLFW_KEY_A) ||
         InputManager::IsKeyDown(GLFW_KEY_S) ||
         InputManager::IsKeyDown(GLFW_KEY_D)) &&
        InputManager::IsKeyDown(GLFW_KEY_LEFT_SHIFT)) {
        GetComponent<AnimationPlayer>()->play("run");
    } else if (InputManager::IsKeyDown(GLFW_KEY_W) ||
               InputManager::IsKeyDown(GLFW_KEY_A) ||
               InputManager::IsKeyDown(GLFW_KEY_S) ||
               InputManager::IsKeyDown(GLFW_KEY_D)) {
        GetComponent<AnimationPlayer>()->play("walk");
    } else {
        GetComponent<AnimationPlayer>()->play("idle");
    }
}