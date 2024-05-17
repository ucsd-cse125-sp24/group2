#include "AssetManager.hpp"


void AssetManager::AddMapping(std::string modelPath, std::vector<AnimationClip*> clips) {
    modelPathToClips.emplace(modelPath, clips);
}

void AssetManager::AddClipToMapping(std::string modelPath, AnimationClip* clip) {
    modelPathToClips[modelPath].push_back(clip);
}

std::vector<AnimationClip*> AssetManager::GetClips(std::string modelPath) {
    return modelPathToClips[modelPath];
}