#pragma once

#include "core.h"
#include <string>
#include <vector>
#include <iostream>

#define MAX_BONE_INFLUENCE 4
struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    // the inverse bind matrix
    glm::mat4 offsetMtx;

};

struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 texCoords;
    // // tangent
    // glm::vec3 Tangent;
    // // bitangent
    // glm::vec3 Bitangent;
	//bone indexes which will influence this vertex
	int boneIDs[MAX_BONE_INFLUENCE];
	//weights from each bone
	float weights[MAX_BONE_INFLUENCE];
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
    // mesh Data
    std::vector<Vertex>       vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture>      textures;
    GLuint VAO;
    void binding();
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setScale(glm::vec3 scale);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    void update(float dt, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale);
private:
    GLuint VBO, EBO;
    glm::mat4 modelMtx;
    glm::vec3 color;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    // glm::quat rotation;
};