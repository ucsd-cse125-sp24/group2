#pragma once

#include "core.h"
#include "AnimationClip.h"
#include "../../_common/include/IComponent.hpp"

class AnimationClip;
class AssimpNodeData;
class Model;
class AnimationPlayer : public IComponent {

private:
    std::vector<glm::mat4> finalBoneMtx;
    std::map<std::string, AnimationClip*> animations;
    float currentTime;
    float deltaTime;
    Model* model;

public:
    AnimationClip* currentAnimation;
    AnimationPlayer(GameObject* owner);
    AnimationPlayer(GameObject* owner, Model* model);

    void AddClip(AnimationClip* clip);
    AnimationClip* GetClip(std::string name);
    void update(float dt);
    void play(AnimationClip* clip);
    void play(std::string name);
    void calculateBoneTransforms(const AssimpNodeData* node,
                                 glm::mat4 parentTransform);
    const std::vector<glm::mat4>& getFinalBoneMatrices() const;

    std::string ToString() override;
};