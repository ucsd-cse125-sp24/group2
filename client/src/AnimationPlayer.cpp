#include "AnimationPlayer.h"

AnimationPlayer::AnimationPlayer() {}

AnimationPlayer::AnimationPlayer(AnimationClip* clip) : IComponent() {
    currentTime = 0.0f;
    deltaTime = 0.0f;
    currentAnimation = clip;
    finalBoneMtx.reserve(100);
    for (int i = 0; i < 100; i++) {
        finalBoneMtx.push_back(glm::mat4(1.0f));
    }
}

AnimationPlayer::AnimationPlayer(std::string path, Model* model) : IComponent() {
    currentTime = 0.0f;
    deltaTime = 0.0f;
    finalBoneMtx.reserve(100);
    for(int i = 0; i < 100; i++) {
        finalBoneMtx.push_back(glm::mat4(1.0f));
    }
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate);
    assert(scene && scene->mRootNode);
    std::cout<<"num animations: " << scene->mNumAnimations << std::endl;
    animations = std::map<std::string, AnimationClip*>();
    for(int i = 0; i < scene->mNumAnimations; i++) {
        aiAnimation* animation = scene->mAnimations[i];
        AnimationClip* clip = new AnimationClip(animation, model, scene);
        animations[clip->getName()] = clip;
        // set default idle animation
        if(i == 0) {
            currentAnimation = clip;
        }
    }
}

void AnimationPlayer::update(float dt) {
    deltaTime = dt;
    // std::cout<<"DeltaTime: " << deltaTime << std::endl;
    if (currentAnimation) {
        currentTime += currentAnimation->getTicksPerSecond() * deltaTime;
        currentTime = fmod(currentTime, currentAnimation->getDuration());
        calculateBoneTransforms(&currentAnimation->getRootNode(),
                                glm::mat4(1.0f));
    }
}

void AnimationPlayer::play(AnimationClip* clip) {
    currentTime = 0.0f;
    currentAnimation = clip;
}

void AnimationPlayer::play(std::string name) {
    currentAnimation = animations[name];
}

void AnimationPlayer::calculateBoneTransforms(const AssimpNodeData* node,
                                              glm::mat4 parentTransform) {
    std::string nodeName = node->name;
    // std::cout<<"node name: "<<nodeName<<std::endl;
    glm::mat4 nodeTransform = node->transformation;
    Bone* bone = currentAnimation->findBone(nodeName);
    if (bone) {
        bone->update(currentTime);
        nodeTransform = bone->getLocalTransform();
    }
    glm::mat4 globalTransform = parentTransform * nodeTransform;
    const auto& boneInfoMap = currentAnimation->getBoneInfoMap();
    if (boneInfoMap.find(nodeName) != boneInfoMap.end()) {
        int index = boneInfoMap.at(nodeName).id;
        finalBoneMtx[index] = currentAnimation->getGlobalInverseTransform() *
                              globalTransform *
                              boneInfoMap.at(nodeName).offsetMtx;
    }
    for (int i = 0; i < node->children.size(); i++) {
        calculateBoneTransforms(&node->children[i], globalTransform);
    }
}

const std::vector<glm::mat4>& AnimationPlayer::getFinalBoneMatrices() const {
    return finalBoneMtx;
}

std::string AnimationPlayer::ToString() { return "AnimationPlayer"; }