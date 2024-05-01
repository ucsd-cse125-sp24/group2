#pragma once

#include "core.h"
#include "AnimationClip.h"

class AnimationPlayer {
public:
    AnimationPlayer();
    AnimationPlayer(AnimationClip* clip);
    void update(float dt);
    void play(AnimationClip* clip);
    void calculateBoneTransforms(const AssimpNodeData* node, glm::mat4 parentTransform);
    const std::vector<glm::mat4>& getFinalBoneMatrices() const;
private:
    std::vector<glm::mat4> finalBoneMtx;
    AnimationClip* currentAnimation;
    float currentTime;
    float deltaTime;
};