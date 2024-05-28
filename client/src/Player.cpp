#include "Player.hpp"

#include "Mover.hpp"
#include "Health.hpp"
#include "components/Model.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "InputManager.h"
#include "components/RendererComponent.hpp"

Player::Player(std::string path, int networkId) : Entity(networkId) {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    RendererComponent* meshRenderer =
        new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(meshRenderer);
    Model* model = new Model(this, path, true);
    AddComponent(model);
    AnimationClip* animationClip = new AnimationClip(this, path, model);
    AddComponent(animationClip);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
    Health* h = new Health(this);
    AddComponent(h);
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