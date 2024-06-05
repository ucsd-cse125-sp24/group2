#include "PlayerEffects.hpp"

PlayerEffects::PlayerEffects() {
    attackEffect = new ParticleSystem(glm::vec3(2000.0f), 10.0f, 30.0f, 20, 0.01, "note_5.png", "../assets/effects", ParticleType::LINE, false);
    healEffect = new ParticleSystem(glm::vec3(0.0f), 1.0f, 3.0f, 10, 0.1, "green4.png", "../assets/effects", ParticleType::SPREAD, true);
    speedUpEffect = new ParticleSystem(glm::vec3(0.0f), 1.0f, 5.0f, 10, 0.1, "up3.png", "../assets/effects", ParticleType::SPREAD, true);
}

PlayerEffects::~PlayerEffects() {
    delete attackEffect;
    delete speedUpEffect;
    delete healEffect;
}

void PlayerEffects::playAttack(glm::vec3 startPosition, glm::vec3 targetPosition, float deltaTime, float duration) {
    acttackEffectActive = true;
    attackEffect->setData(startPosition, targetPosition, attackEffect->getVelocity());
    attackEffect->start(deltaTime, ParticleType::LINE, duration);
}

void PlayerEffects::playSpeedUp(glm::vec3 position, float deltaTime, float duration) {
    speedUpEffectActive = true;
    speedUpEffect->setData(position, glm::vec3(0.0f), speedUpEffect->getVelocity());
    speedUpEffect->start(deltaTime, ParticleType::SPREAD, duration);

}

void PlayerEffects::playHeal(glm::vec3 position, float deltaTime, float duration) {
    healEffectActive = true;
    healEffect->setData(position, glm::vec3(0.0f), healEffect->getVelocity());
    healEffect->start(deltaTime, ParticleType::SPREAD, duration);
}

void PlayerEffects::update(float deltaTime) {
    if(acttackEffectActive) {
        attackEffect->update(deltaTime);
    }
    if(healEffectActive) {
        healEffect->update(deltaTime);
    }
    if(speedUpEffectActive) {
        speedUpEffect->update(deltaTime);
    }
}

void PlayerEffects::draw(const glm::mat4& viewMtx, const glm::mat4& projMtx) {
    if(acttackEffectActive) {
        attackEffect->draw(viewMtx, projMtx);
    }
    if(healEffectActive) {
        healEffect->draw(viewMtx, projMtx);
    }
    if(speedUpEffectActive) {
        speedUpEffect->draw(viewMtx, projMtx);
    }
}