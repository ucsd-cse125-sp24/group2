#ifndef PLAYER_EFFECTS_H
#define PLAYER_EFFECTS_H

#include "ParticleSystem.hpp"

class PlayerEffects {
public:
    PlayerEffects();
    ~PlayerEffects();
    void playAttack(glm::vec3 startPosition, glm::vec3 targetPosition, float deltaTime, float duration);
    void playSpeedUp(glm::vec3 position, float deltaTime, float duration);
    void playHeal(glm::vec3 position, float deltaTime, float duration);
    void update(float deltaTime);
    void draw(const glm::mat4& viewMtx, const glm::mat4& projMtx);
    void setAttackEffectActive(bool active) { acttackEffectActive = active; }
    void setSpeedUpEffectActive(bool active) { speedUpEffectActive = active; }
    void setHealEffectActive(bool active) { healEffectActive = active; }
private:
    bool acttackEffectActive = false;
    bool speedUpEffectActive = false;
    bool healEffectActive = false;
    ParticleSystem* attackEffect;
    ParticleSystem* speedUpEffect;
    ParticleSystem* healEffect;
};

#endif