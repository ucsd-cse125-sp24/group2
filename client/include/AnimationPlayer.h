#pragma once

#include "core.h"
#include "AnimationClip.h"
#include "../../_common/include/IComponent.hpp"

class AnimationClip;
class AssimpNodeData;
class AnimationPlayer : public IComponent {

private:
    std::vector<glm::mat4> finalBoneMtx;
    AnimationClip* currentAnimation;
    float currentTime;
    float deltaTime;

public:
    AnimationPlayer();
    AnimationPlayer(AnimationClip* clip);

    void update(float dt);
    void play(AnimationClip* clip);
    void calculateBoneTransforms(const AssimpNodeData* node,
                                 glm::mat4 parentTransform);
    const std::vector<glm::mat4>& getFinalBoneMatrices() const;

    std::string ToString() override;
};