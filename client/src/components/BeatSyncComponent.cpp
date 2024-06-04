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

void BeatSyncComponent::Update(float deltaTime) {
    glm::vec3 newScale = owner->GetComponent<Transform>()->GetScale();
    newScale -= glm::vec3(1, 1, 1) * deltaTime * 50.0f;
    newScale =
        glm::clamp(newScale, glm::vec3(25, 25, 25), glm::vec3(50, 50, 50));

    owner->GetComponent<Transform>()->SetScale(newScale);
}