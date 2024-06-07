#include "components/BeatSyncComponent.hpp"
#include "AudioManager.hpp"
#include "EventArgs.hpp"
#include <Transform.hpp>

BeatSyncComponent::BeatSyncComponent() {
    AudioManager::instance().Beat += [this](EventArgs* e) {
        BeatEventArgs* args = (BeatEventArgs*)e;
        if (args->beat % 2 == 0)
            owner->GetComponent<Transform>()->SetScale(glm::vec3(50, 50, 50));
    };
}

BeatSyncComponent::BeatSyncComponent(bool scaleYOnly) {
    if(scaleYOnly) {
        AudioManager::instance().Beat += [this](EventArgs* e) {
            BeatEventArgs* args = (BeatEventArgs*)e;
            if (args->beat % 2 == 0)
                owner->GetComponent<Transform>()->SetScale(glm::vec3(0, 30, 0));
        };
        scaleYOnly = true;
    }
    
}

void BeatSyncComponent::Update(float deltaTime) {
     glm::vec3 newScale = owner->GetComponent<Transform>()->GetScale();
    if(scaleYOnly) {
        newScale -= glm::vec3(1, 30, 1) * deltaTime;
        newScale =
            glm::clamp(newScale, glm::vec3(0, 15, 0), glm::vec3(0, 30, 0));

    } else {
    newScale -= glm::vec3(1, 1, 1) * deltaTime * 50.0f;
    newScale =
        glm::clamp(newScale, glm::vec3(25, 25, 25), glm::vec3(50, 50, 50));
    }
    owner->GetComponent<Transform>()->SetScale(newScale);
}