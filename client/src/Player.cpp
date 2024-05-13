#include "Player.h"
#include "InputManager.h"
#include "AnimationPlayer.h"

void Player::update() {
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
        // velocityHeading = glm::vec3(0);
    }
}