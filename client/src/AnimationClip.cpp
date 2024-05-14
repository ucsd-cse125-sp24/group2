#include "AnimationClip.h"

AnimationClip::AnimationClip(GameObject* owner, std::string path, Model* model)
    : IComponent(owner) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    // assuming there is only one animation in the scene rn
    auto animation = scene->mAnimations[0];
    duration = animation->mDuration;
    ticksPerSecond = animation->mTicksPerSecond;
    aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
    globalTransformation = globalTransformation.Inverse();
    globalInverseTransform =
        Helper::ConvertMatrixToGLMFormat(globalTransformation);
    readHierarchyData(rootNode, scene->mRootNode);
    readMissingBones(animation, model);
}

AnimationClip::AnimationClip(aiAnimation* clip, Model* model,
                             const aiScene* scene) {
    duration = clip->mDuration;
    name = clip->mName.C_Str();
    std::cout << "animation name: " << name << std::endl;
    ticksPerSecond = clip->mTicksPerSecond;
    aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
    globalTransformation = globalTransformation.Inverse();
    globalInverseTransform =
        Helper::ConvertMatrixToGLMFormat(globalTransformation);
    readHierarchyData(rootNode, scene->mRootNode);
    readMissingBones(clip, model);
}

Bone* AnimationClip::findBone(const std::string& name) {
    auto iter = std::find_if(bones.begin(), bones.end(), [&](const Bone& Bone) {
        return Bone.getName() == name;
    });
    if (iter == bones.end())
        return nullptr;
    else
        return &(*iter);
}

float AnimationClip::getDuration() const { return duration; }

int AnimationClip::getTicksPerSecond() const { return ticksPerSecond; }

const AssimpNodeData& AnimationClip::getRootNode() const { return rootNode; }

const std::map<std::string, BoneInfo>& AnimationClip::getBoneInfoMap() const {
    return boneInfoMap;
}

void AnimationClip::readMissingBones(const aiAnimation* animation,
                                     Model* model) {
    int size = animation->mNumChannels;
    auto& boneInfoMap = model->getBoneInfoMap();
    int boneCount = model->getBoneCount();
    for (int i = 0; i < size; i++) {
        auto channel = animation->mChannels[i];
        std::string boneName = channel->mNodeName.data;
        if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
            boneInfoMap[boneName].id = boneCount;
            model->addBoneCount();
        }
        bones.push_back(Bone(channel->mNodeName.data,
                             boneInfoMap[channel->mNodeName.data].id, channel));
    }

    this->boneInfoMap = boneInfoMap;
}

void AnimationClip::readHierarchyData(AssimpNodeData& dest, const aiNode* src) {
    assert(src);

    dest.name = src->mName.data;
    dest.transformation =
        Helper::ConvertMatrixToGLMFormat(src->mTransformation);

    for (int i = 0; i < src->mNumChildren; i++) {
        AssimpNodeData newData;
        readHierarchyData(newData, src->mChildren[i]);
        dest.children.push_back(newData);
        // std::cout<<"newData: "<<newData.name<<std::endl;
    }
}

const glm::mat4& AnimationClip::getGlobalInverseTransform() const {
    return globalInverseTransform;
}

std::string AnimationClip::ToString() { return "AnimationClip"; }

std::string AnimationClip::getName() const { return name; }