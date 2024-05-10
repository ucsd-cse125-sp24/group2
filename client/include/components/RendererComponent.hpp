#pragma once
#include "IComponent.hpp"
#define GLEW_STATIC
#include <GL/glew.h>
#include "Shader.h"

class RendererComponent : public IComponent {
    private:
        ShaderType type;

    public:
        RendererComponent(GameObject* owner, ShaderType type) : IComponent(owner), type(type) {}
        void Render(glm::mat4 view);
        std::string ToString() {}
};