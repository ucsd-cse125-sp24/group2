#ifndef SKY_BOX_H
#define SKY_BOX_H

#include "core.h"
#include "Helper.h"
#include <vector>
#include <stb_image.h>
#include "Shader.h"
#include <iostream>

class SkyBox{
public:
    SkyBox();
    ~SkyBox();
    void draw(const glm::mat4& viewMtx, const glm::mat4& projMtx);
private:
    unsigned int loadCubemap();
    unsigned int VAO, VBO;
    unsigned int shader;
    unsigned int textureID;
    glm::mat4 modelMtx;
    std::vector<std::string> faces;
};

#endif