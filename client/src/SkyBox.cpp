#include "SkyBox.hpp"

SkyBox::SkyBox() {
    shader = Shader::GetShader(ShaderType::SKYBOX);
    modelMtx = glm::mat4(1.0f);
    float skyboxVertices[] = {
        // positions          
        -50.0f,  50.0f, -50.0f,
        -50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,

        -50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f, -50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,

         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,

        -50.0f, -50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f, -50.0f,  50.0f,
        -50.0f, -50.0f,  50.0f,

        -50.0f,  50.0f, -50.0f,
         50.0f,  50.0f, -50.0f,
         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f,  50.0f,
        -50.0f,  50.0f, -50.0f,

        -50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f,
        -50.0f, -50.0f,  50.0f,
         50.0f, -50.0f,  50.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    faces = {
        "../assets/skybox/px.png",  // right
        "../assets/skybox/nx.png",  // left
        "../assets/skybox/py.png",  // top
        "../assets/skybox/ny.png",  // bottom
        "../assets/skybox/pz.png",  // front
        "../assets/skybox/nz.png"   // back
    };

    // faces = {
    //     "../assets/skybox2/right.jpg",  // right
    //     "../assets/skybox2/left.jpg",  // left
    //     "../assets/skybox2/top.jpg",  // top
    //     "../assets/skybox2/bottom.jpg",  // bottom
    //     "../assets/skybox2/front.jpg",  // front
    //     "../assets/skybox2/back.jpg"   // back
    // };

    this->textureID = loadCubemap();

}

SkyBox::~SkyBox() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

unsigned int SkyBox::loadCubemap() {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {

            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;

}

void SkyBox::draw(const glm::mat4& viewMtx, const glm::mat4& projMtx) {
    glDepthFunc(GL_LEQUAL);
    glUseProgram(shader);
   // glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, false,
    //                   (float*)&modelMtx);

    glm::mat4 newView = glm::mat4(glm::mat3(viewMtx));
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, false,
                       (float*)&newView);

    glUniformMatrix4fv(glGetUniformLocation(shader, "proj"), 1, false, (float*)&projMtx);
    glBindVertexArray(VAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glUseProgram(0);
    glDepthFunc(GL_LESS);
}