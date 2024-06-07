#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures)
    : vertices(vertices), indices(indices), textures(textures) {
    modelMtx = glm::mat4(1.0f);
    scale = glm::vec3(1.0f);
    // rotation = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);
    // rotation = glm::rotate(rotation, glm::radians(-90.0f), glm::vec3(1.0f,
    // 0.0f, 0.0f));
    // modelMtx = glm::rotate(modelMtx, glm::radians(-90.0f), glm::vec3(1.0f,
    // 0.0f, 0.0f)); modelMtx = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,
    // -50.0f, 0.0f));
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    binding();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices)
    : vertices(vertices), indices(indices) {
    modelMtx = glm::mat4(1.0f);
    scale = glm::vec3(1.0f);
    color = glm::vec3(1.0f, 0.95f, 0.1f);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    binding();
}

void Mesh::binding() {
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
                 &vertices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);
    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, texCoords));
    // vertex bone ids
    glEnableVertexAttribArray(3);
    glVertexAttribIPointer(3, 4, GL_INT, sizeof(Vertex),
                           (void*)offsetof(Vertex, boneIDs));
    // vertex bone weights
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void*)offsetof(Vertex, weights));
}

void Mesh::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // bind appropriate textures
    glUseProgram(shader);
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int heightNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++) {
        // active proper texture unit before binding
        glActiveTexture(GL_TEXTURE0 + i);
        // retrieve texture number (the N in diffuse_textureN)
        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number =
                std::to_string(specularNr++); // transfer unsigned int to string
        else if (name == "texture_normal")
            number =
                std::to_string(normalNr++); // transfer unsigned int to string
        else if (name == "texture_height")
            number =
                std::to_string(heightNr++); // transfer unsigned int to string

        // now set the sampler to the correct texture unit
        glUniform1i(glGetUniformLocation(shader, (name + number).c_str()), i);
        // and finally bind the texture
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glUniform1i(glGetUniformLocation(shader, "lightOn"), lightOn);
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false,
                       (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE,
                       (float*)&modelMtx);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
    glUseProgram(0);
}

void Mesh::setPosition(glm::vec3 pos) { position = pos; }

void Mesh::setRotation(glm::vec3 rot) { rotation = rot; }

void Mesh::update(float dt, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    position = pos;
    rotation = rot;
    this->scale = scale;
    // std::cout<<"Scale in mesh: " << glm::to_string(this->scale) << std::endl;
    // std::cout<<"position: "<<glm::to_string(position)<<std::endl;
    modelMtx = glm::translate(position) * glm::scale(glm::mat4(1.0f), this->scale);
    // modelMtx = glm::translate(position);
    // std::cout<<"position matrix" << glm::to_string(modelMtx) << std::endl;
    modelMtx = glm::rotate(modelMtx, glm::radians(rot.y), glm::vec3(0, 1, 0));
    modelMtx = glm::rotate(modelMtx, glm::radians(rot.x), glm::vec3(1, 0, 0));
    modelMtx = glm::rotate(modelMtx, glm::radians(rot.z), glm::vec3(0, 0, 1));
}

void Mesh::setScale(glm::vec3 scale) { this->scale = scale; }