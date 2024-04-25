#pragma once

#include "core.h"
#include <string>
#include <iostream>
#include "Mesh.h"
#include "stb_image.h"

class Model {
public:
    Model();
    Model(std::string path);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
private:
    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
    unsigned int textureFromFile(const char *path, const std::string &directory);

    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
};