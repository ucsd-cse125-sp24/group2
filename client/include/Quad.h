#pragma once
#include "core.h"
#include "Shader.h"
#include "Helper.h"
#include "../../_common/include/States.hpp"

class Quad : public States{
public:
    Quad();
    Quad(glm::vec3 pos, float size);
    Quad(glm::vec3 pos, float width, float height);
    ~Quad();
    void draw(float aspectRatio);
    void draw(const glm::mat4& viewProjMtx);
    void update();
    void setRotation(float angle, glm::vec3 axis);
    void setPosition(glm::vec3 pos);
    void setTexture(const char* path, const std::string& directory);
    void setSize(float size);
    void setSize(float width, float height);
    void setColor(glm::vec4 color);
    void setShader(GLuint shader);
    GLuint getShader() { return shader; }
    const glm::mat4& getModelMtx();
    const glm::vec3& getPosition() const { return position; }
    glm::mat4 modelMtx;
private:
    std::vector<unsigned int> textures;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<unsigned int> indices;
    GLuint VBO_Pos, VBO_TexCoods, EBO;
    glm::vec3 position;
    glm::quat rotation;
    float size;
    GLuint VAO;
    GLuint shader;
    glm::vec3 scale;
    glm::vec4 color;
    bool hasTexture = false;
};
