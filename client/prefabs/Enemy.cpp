#include "Enemy.hpp"

#include "NetTransform.hpp"
#include "components/Model.h"
#include "AnimationPlayer.h"
#include "components/RendererComponent.hpp"
#include "Health.hpp"


Enemy::Enemy(std::string path, int networkId) : Entity(networkId) {
    Model* model = new Model(this, path, true);
    AddComponent(model);
    GetComponent<NetTransform>()->SetScale(glm::vec3(400.0f));
    AnimationPlayer* animationPlayer = new AnimationPlayer(this, model);
    AddComponent(animationPlayer);
    RendererComponent* renderer = new RendererComponent(this, ShaderType::ANIMATED);
    AddComponent(renderer);
    Health* health = new Health(this);
    AddComponent(health);
    // create enemy animation controller component
    animationPlayer->play("idle");
}

void Enemy::update(float deltaTime) {}