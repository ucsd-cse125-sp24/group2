#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <string>
#include "AnimationPlayer.h"
#include "AnimationClip.h"

void RendererComponent::Render(glm::mat4 view) {
    // Render animations, if applicable
    if (auto anim = owner->GetComponent<AnimationPlayer>()) {
        auto transforms = anim->getFinalBoneMatrices();
        glUseProgram(Shader::GetShader(type));
        for (int i = 0; i < transforms.size(); i++) {
            glUniformMatrix4fv(
                glGetUniformLocation(
                    Shader::GetShader(type),
                    ("finalBonesMatrices[" + std::to_string(i) + "]").c_str()),
                1, GL_FALSE, glm::value_ptr(transforms[i]));
        }
    }

    Model* modelComponent = owner->GetComponent<Model>();
    modelComponent->draw(view, Shader::GetShader(type));
}