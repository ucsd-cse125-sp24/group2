#include "Player.hpp"

#include "Mover.hpp"
#include "MovementStateMachine.hpp"
#include "components/Model.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "AssetManager.hpp"
#include "InputManager.h"
#include "components/RendererComponent.hpp"
#include "Status.hpp"

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
    Status* status = new Status(this);
    AddComponent(status);
}

void Player::update(float deltaTime) {
}