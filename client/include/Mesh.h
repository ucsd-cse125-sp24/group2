#pragma once

#include "core.h"
#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 4

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
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
private:
    GLuint VBO, EBO;
    glm::mat4 modelMtx;
    glm::vec3 color;
};