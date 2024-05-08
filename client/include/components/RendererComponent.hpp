#pragma once
#include "IComponent.hpp"
#include <GL/glew.h>

class RendererComponent : public IComponent {
public:
    void Render(glm::mat4 view, GLuint shaderProgram);
};