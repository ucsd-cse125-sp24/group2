#pragma once

#include "core.h"
#include "components/Model.h"
#include "Bone.h"
#include "IComponent.hpp"

struct AssimpNodeData {
    glm::mat4 transformation;
    std::string name;
    std::vector<AssimpNodeData> children;
};

class Model;

class AnimationClip : public IComponent {

private:
    float duration;
    int ticksPerSecond;

    std::vector<Bone> bones;
    AssimpNodeData rootNode;
    std::map<std::string, BoneInfo> boneInfoMap;
    glm::mat4 globalInverseTransform;
    std::string name;

    void readMissingBones(const aiAnimation* animation, Model* model);
    void readHierarchyData(AssimpNodeData& dest, const aiNode* src);

public:
    AnimationClip() = default;
    AnimationClip(std::string path, Model* model);
    AnimationClip(aiAnimation* clip, Model* model, const aiScene* scene);

    Bone* findBone(const std::string& name);
    float getDuration() const;
    std::string getName() const;
    int getTicksPerSecond() const;
    const AssimpNodeData& getRootNode() const;
    const std::map<std::string, BoneInfo>& getBoneInfoMap() const;
    const glm::mat4& getGlobalInverseTransform() const;

    std::string ToString() override;
};