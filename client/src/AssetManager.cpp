#include "AssetManager.hpp"

void AssetManager::AddMapping(std::string modelPath, Model* model,
                              std::vector<AnimationClip*> clips) {
    modelPathToClips.emplace(
        modelPath,
        std::pair<Model*, std::vector<AnimationClip*>>(model, clips));
}

void AssetManager::AddClipToMapping(std::string modelPath,
                                    AnimationClip* clip) {
    modelPathToClips[modelPath].second.push_back(clip);
}

std::vector<AnimationClip*> AssetManager::GetClips(std::string modelPath) {
    return modelPathToClips[modelPath].second;
}

Model* AssetManager::GetModel(std::string modelPath) {
    return modelPathToClips[modelPath].first;
}