#include "components/RendererComponent.hpp"
#include "components/Model.h"
#include <string>

void RendererComponent::Render(glm::mat4 view, GLuint shaderProgram) {
    Model* modelComponent = owner->GetComponent<Model>();
    modelComponent->draw(view, shaderProgram);

    // TODO animation component
}