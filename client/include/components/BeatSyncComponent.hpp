#pragma once
#include "IComponent.hpp"

class BeatSyncComponent : public IComponent {
public:
    BeatSyncComponent();
    BeatSyncComponent(bool scaleYOnly);
    virtual void Update(float deltaTime) override;
    std::string ToString() override { return ""; }
    bool scaleYOnly = false;
};