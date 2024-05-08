#pragma once

#include <string>
#include <map>
#include <iostream>

#include "core.h"
#include "Mesh.h"
#include "stb_image.h"
#include "Helper.h"

#include "IComponent.hpp"
#include "AnimationClip.h"
#include "AnimationPlayer.h"

class AnimationPlayer;
class AnimationClip;
class Model : public IComponent {

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    std::map<std::string, BoneInfo> boneInfoMap;
    int boneCounter = 0;

    void loadModel(std::string path);
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat,
                                              aiTextureType type,
                                              std::string typeName);
    unsigned int textureFromFile(const char* path,
                                 const std::string& directory);
    void setVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
    void extractBoneWeightForVertices(std::vector<Vertex>& vertices,
                                      aiMesh* mesh, const aiScene* scene);

public:
    AnimationPlayer* animationPlayer;
    AnimationClip* clip;
    Model();
    Model(std::string path);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    int getBoneCount() const;
    std::map<std::string, BoneInfo>& getBoneInfoMap();
    void addBoneCount();
    void update(float dt, glm::vec3 pos);
    void setPosition(glm::vec3 pos);

    std::string ToString() override;
};