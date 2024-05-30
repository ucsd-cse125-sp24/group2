#pragma once
#include "IComponent.hpp"

class BeatSyncComponent : public IComponent {
public:
    BeatSyncComponent();
    virtual void Update(float deltaTime) override;
    std::string ToString() override { return ""; }
};