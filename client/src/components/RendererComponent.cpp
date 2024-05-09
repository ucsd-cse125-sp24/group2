#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <string>
#include "AnimationPlayer.h"
#include "AnimationClip.h"

void RendererComponent::Render(glm::mat4 view) {
    Model* modelComponent = owner->GetComponent<Model>();
    modelComponent->draw(view, Shader::GetShader(type));

    // TODO check if it has animationplayer
    auto transforms =
        owner->GetComponent<AnimationPlayer>()->getFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); i++) {
        glUseProgram(Shader::GetShader(type));
        glUniformMatrix4fv(
            glGetUniformLocation(
                Shader::GetShader(type),
                ("finalBonesMatrices[" + std::to_string(i) + "]").c_str()),
            1, GL_FALSE, glm::value_ptr(transforms[i]));
    }
}