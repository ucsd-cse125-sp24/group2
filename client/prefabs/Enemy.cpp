#include "Enemy.hpp"

#include "NetTransform.hpp"
#include "components/Model.h"
#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "Health.hpp"
#include "EnemyComponent.hpp"
#include <AssetManager.hpp>

Enemy::Enemy(std::string path, int networkId) : Entity(networkId) {
    // Model* model = new Model(this, path, true);
    Model* model = new Model(AssetManager::Instance().GetModel(path));
    AddComponent(model);
    GetComponent<NetTransform>()->SetScale(glm::vec3(50.0f));
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
    RendererComponent* renderer =
        new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(renderer);
    Health* health = new Health(this);
    AddComponent(health);
    EnemyComponent* ec = new EnemyComponent(this);
    AddComponent(ec);
}

void Enemy::update(float deltaTime) {}