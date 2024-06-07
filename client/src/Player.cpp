#include "Player.hpp"

#include "Mover.hpp"
#include "Health.hpp"
#include "components/Model.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "InputManager.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "components/PlayerComponent.hpp"
#include "HUD.h"
#include "MovementStateMachine.hpp"
#include "Status.hpp"

Player::Player(std::string path, int networkId) : Entity(networkId) {
    alive = true;
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
    PlayerComponent* playerComponent = new PlayerComponent(this);
    AddComponent(playerComponent);
    Health* h = new Health(this);
    AddComponent(h);
    Status* status = new Status(this);
    AddComponent(status);
}

void Player::update(float deltaTime) {}

void Player::onDestroy() {}