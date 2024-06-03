#include "Cube.h"

Cube::Cube(glm::vec3 cubeMin, glm::vec3 cubeMax) {
    // Model matrix.
    model = glm::mat4(1.0f);
    position = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    rotation = glm::vec3(0.0f);

    // The color of the cube. Try setting it to something else!
    color = glm::vec3(1.0f, 0.95f, 0.1f);

    // Specify vertex positions
    // positions = {
    //     // Front
    //     glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
    //     glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
    //     glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
    //     glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),

    //     // Back
    //     glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
    //     glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
    //     glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
    //     glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),

    //     // Top
    //     glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
    //     glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
    //     glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
    //     glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

    //     // Bottom
    //     glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
    //     glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
    //     glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
    //     glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),

    //     // Left
    //     glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
    //     glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
    //     glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z),
    //     glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),

    //     // Right
    //     glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z),
    //     glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z),
    //     glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z),
    //     glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z)};

    positions = {
        glm::vec3(cubeMin.x, cubeMin.y, cubeMin.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMin.z), 
        glm::vec3(cubeMax.x, cubeMax.y, cubeMin.z), 
        glm::vec3(cubeMin.x, cubeMax.y, cubeMin.z),
        glm::vec3(cubeMin.x, cubeMin.y, cubeMax.z),
        glm::vec3(cubeMax.x, cubeMin.y, cubeMax.z), 
        glm::vec3(cubeMax.x, cubeMax.y, cubeMax.z),
        glm::vec3(cubeMin.x, cubeMax.y, cubeMax.z) 
    };

    indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // Bottom face
        4, 5, 5, 6, 6, 7, 7, 4, // Top face
        0, 4, 1, 5, 2, 6, 3, 7  // Vertical edges
    };

    // Specify normals
    // normals = {
    //     // Front
    //     glm::vec3(0, 0, 1),
    //     glm::vec3(0, 0, 1),
    //     glm::vec3(0, 0, 1),
    //     glm::vec3(0, 0, 1),

    //     // Back
    //     glm::vec3(0, 0, -1),
    //     glm::vec3(0, 0, -1),
    //     glm::vec3(0, 0, -1),
    //     glm::vec3(0, 0, -1),

    //     // Top
    //     glm::vec3(0, 1, 0),
    //     glm::vec3(0, 1, 0),
    //     glm::vec3(0, 1, 0),
    //     glm::vec3(0, 1, 0),

    //     // Bottom
    //     glm::vec3(0, -1, 0),
    //     glm::vec3(0, -1, 0),
    //     glm::vec3(0, -1, 0),
    //     glm::vec3(0, -1, 0),

    //     // Left
    //     glm::vec3(-1, 0, 0),
    //     glm::vec3(-1, 0, 0),
    //     glm::vec3(-1, 0, 0),
    //     glm::vec3(-1, 0, 0),

    //     // Right
    //     glm::vec3(1, 0, 0),
    //     glm::vec3(1, 0, 0),
    //     glm::vec3(1, 0, 0),
    //     glm::vec3(1, 0, 0)};

    // Specify indices
    // indices = {
    //     0, 1, 2, 0, 2, 3,        // Front
    //     4, 5, 6, 4, 6, 7,        // Back
    //     8, 9, 10, 8, 10, 11,     // Top
    //     12, 13, 14, 12, 14, 15,  // Bottom
    //     16, 17, 18, 16, 18, 19,  // Left
    //     20, 21, 22, 20, 22, 23,  // Right
    // };

    // Generate a vertex array (VAO) and two vertex buffer objects (VBO).
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_positions);
    // glGenBuffers(1, &VBO_normals);

    // Bind to the VAO.
    glBindVertexArray(VAO);

    // Bind to the first VBO - We will use it to store the vertices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Bind to the second VBO - We will use it to store the normals
    // glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(), normals.data(), GL_STATIC_DRAW);
    // glEnableVertexAttribArray(1);
    // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

    // Generate EBO, bind the EBO to the bound VAO and send the data
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

    // Unbind the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Cube::~Cube() {
    // Delete the VBOs and the VAO.
    glDeleteBuffers(1, &VBO_positions);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

void Cube::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    // actiavte the shader program
    glUseProgram(shader);

    // get the locations and send the uniforms to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader, "viewProj"), 1, false, (float*)&viewProjMtx);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, (float*)&model);
    glUniform3fv(glGetUniformLocation(shader, "DiffuseColor"), 1, &color[0]);

    // Bind the VAO
    glBindVertexArray(VAO);

    // draw the points using triangles, indexed with the EBO
    glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);

    // Unbind the VAO and shader program
    glBindVertexArray(0);
    glUseProgram(0);
}

void Cube::update(float dt, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    // Spin the cube
    position = pos;
    rotation = rot;
    this->scale = scale;
    // std::cout<<"position: "<<glm::to_string(position)<<std::endl;
    // std::cout<<"rotation: "<<glm::to_string(rotation)<<std::endl;
    // std::cout<<"scale: "<<glm::to_string(this->scale)<<std::endl;

    // std::cout<<"Scale in mesh: " << glm::to_string(this->scale) << std::endl;
    // std::cout<<"position: "<<glm::to_string(position)<<std::endl;
    model = glm::translate(position) * glm::scale(glm::mat4(1.0f), this->scale);
    // modelMtx = glm::translate(position);
    // std::cout<<"position matrix" << glm::to_string(modelMtx) << std::endl;
    model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0, 0, 1));
}

void Cube::spin(float deg) {
    // Update the model matrix by multiplying a rotation matrix
    model = model * glm::rotate(glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& Cube::getModelMtx() {
    return model;
}

void Cube::setPosition(glm::vec3 pos) {
    position = pos;
}

void Cube::setRotation(glm::vec3 rot) {
    rotation = rot;

}
void Cube::setScale(glm::vec3 scale) {
    this->scale = scale;
}