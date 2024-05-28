#include "Player.hpp"

#include "Mover.hpp"
#include "components/Model.h"
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "InputManager.h"
#include "components/RendererComponent.hpp"
#include "AssetManager.hpp"
#include "components/PlayerComponent.hpp"
#include "HUD.h"

Player::Player(std::string path, int networkId) : Entity(networkId) {
    Mover* mover = new Mover(this);
    AddComponent(mover);
    RendererComponent* meshRenderer =
        new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(meshRenderer);
    Model* model = new Model(AssetManager::Instance().GetModel(path));
    AddComponent(model);
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
    PlayerComponent* playerComponent = new PlayerComponent(this);
    AddComponent(playerComponent);
    HUDs* hudComponent = new HUDs(this);
    AddComponent(hudComponent);
    
}

void Player::update(float deltaTime) {}