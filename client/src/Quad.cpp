#include "Quad.h"

Quad::Quad() {

}

Quad::Quad(glm::vec3 pos, float width, float height) : position(pos){
    modelMtx = glm::mat4(1.0f);
    scale = glm::vec3(width, height, 1.0f);
    shader = Shader::GetShader(ShaderType::HUD);
    rotation = glm::quat(0, 0, 0, 0);
    positions = {
        glm::vec3(-1.0f - width/2.0f, -1.0f - height/2.0f, 0.0f), // bottom left
        glm::vec3(-1.0f - width/2.0f, 1.0f + height/2.0f, 0.0f), // top left
        glm::vec3(1.0f + width/2.0f, -1.0f - height/2.0f, 0.0f), // bottom right
        glm::vec3(1.0f + width/2.0f, 1.0f + height/2.0f, 0.0f) // top right
    };
    indices = {
        0, 1, 2,
        2, 1, 3
    };

    texCoords = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_Pos);
    glGenBuffers(1, &VBO_TexCoods);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoods);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

Quad::Quad(glm::vec3 pos, float size) : position(pos){
    modelMtx = glm::mat4(1.0f);
    scale = glm::vec3(size, size, 1.0f);
    shader = Shader::GetShader(ShaderType::HUD);
    positions = {
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3(-1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, -1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f)
    };
    indices = {
        0, 1, 2,
        2, 1, 3
    };

    texCoords = {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(1.0f, 1.0f)
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_Pos);
    glGenBuffers(1, &VBO_TexCoods);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_Pos);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_TexCoods);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);
}

Quad::~Quad() {
    glDeleteBuffers(1, &VBO_Pos);
    glDeleteBuffers(1, &VBO_TexCoods);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
    for(unsigned int i = 0; i < textures.size(); i++) {
        glDeleteTextures(1, &textures[i]);
    }
}

void Quad::draw(float aspectRatio) {
    glEnable(GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glUseProgram(shader);
    for(unsigned int i = 0; i < textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        std::string name = "texture" + std::to_string(i);
        // std::cout<<"texture name: "<< (name).c_str() << std::endl;
        glUniform1i(glGetUniformLocation(shader, (name).c_str()), i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
    glUniform1f(glGetUniformLocation(shader, "aspectRatio"), aspectRatio);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&modelMtx);

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

void Quad::setPosition(glm::vec3 pos) {
    position = pos;
}

void Quad::setSize(float size) {
    scale = glm::vec3(size, size, 1.0f);

}

void Quad::setSize(float width, float height) {
    scale = glm::vec3(width, height, 1.0f);
}
    
const glm::mat4& Quad::getModelMtx(){
    modelMtx = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotation) * glm::scale(glm::mat4(1.0f), scale);
    return modelMtx;
}

void Quad::update() {
    getModelMtx();
}

void Quad::setTexture(const char* path, const std::string& directory) {
    unsigned int texture = Helper::textureFromFile(path, directory);
    textures.push_back(texture);
}

void Quad::setRotation(float angle, glm::vec3 axis) {
    rotation =  glm::rotate(rotation, angle, axis);
}