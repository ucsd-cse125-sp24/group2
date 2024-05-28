#pragma once

#include <unordered_map>
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "components/Model.h"
#include <utility>

class AssetManager {
public:
    std::unordered_map<std::string,
                       std::pair<Model*, std::vector<AnimationClip*>>>
        modelPathToClips;

    static AssetManager& Instance() {
        static AssetManager m;
        return m;
    }
    void AddMapping(std::string modelPath, Model* model,
                    std::vector<AnimationClip*> clips);
    void AddClipToMapping(std::string modelPath, AnimationClip* clip);
    std::vector<AnimationClip*> GetClips(std::string modelPath);
    Model* GetModel(std::string modelPath);
};