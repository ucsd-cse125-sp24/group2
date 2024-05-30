#include "components/BeatSyncComponent.hpp"
#include "AudioManager.hpp"
#include "EventArgs.hpp"

BeatSyncComponent::BeatSyncComponent() {
    AudioManager::instance().Beat +=
        [this](EventArgs* e) { BeatEventArgs* args = (BeatEventArgs*)e; };
}

void BeatSyncComponent::Update(float deltaTime) {}
