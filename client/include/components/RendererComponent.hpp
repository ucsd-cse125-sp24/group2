#pragma once
#include "IComponent.hpp"
#include <GL/glew.h>

class RendererComponent : public IComponent {
public:
    RendererComponent(GameObject* owner) : IComponent(owner) {}
    void Render(glm::mat4 view, GLuint shaderProgram);
    std::string ToString() {}
};