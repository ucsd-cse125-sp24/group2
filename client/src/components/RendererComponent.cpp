#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <string>
#include "AnimationPlayer.h"
#include "AnimationClip.h"

void RendererComponent::Render(glm::mat4 view, GLuint shaderProgram) {
    Model* modelComponent = owner->GetComponent<Model>();
    modelComponent->draw(view, shaderProgram);
    auto transforms = modelComponent->animationPlayer->getFinalBoneMatrices();
    for (int i = 0; i < transforms.size(); i++) {
        glUseProgram(shaderProgram);
        glUniformMatrix4fv(
            glGetUniformLocation(
                shaderProgram,
                ("finalBonesMatrices[" + std::to_string(i) + "]").c_str()),
            1, GL_FALSE, glm::value_ptr(transforms[i]));
    }
}