#pragma once


#include <unordered_map>
#include "AnimationClip.h"
#include "AnimationPlayer.h"
#include "components/Model.h"


class AssetManager {
public:
    std::unordered_map<std::string, std::vector<AnimationClip*>> modelPathToClips;

    static AssetManager& Instance() {
        static AssetManager m;
        return m;
    }
    void AddMapping(std::string modelPath, std::vector<AnimationClip*> clips);
    void AddClipToMapping(std::string modelPath, AnimationClip* clip);
    std::vector<AnimationClip*> GetClips(std::string);
};