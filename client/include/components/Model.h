#pragma once

#include <string>
#include <map>
#include <iostream>

#include "core.h"
#include "Mesh.h"

#include "IComponent.hpp"
#include "AnimationClip.h"
#include "AnimationPlayer.h"

#include "Helper.h"
#include "Cube.h"

class AnimationPlayer;
class AnimationClip;
class Model : public IComponent {

private:
    std::vector<Mesh> meshes;
    std::string directory;
    std::vector<Texture> textures_loaded;
    std::map<std::string, BoneInfo> boneInfoMap;
    int boneCounter = 0;
    bool hasAnimation = false;
    const aiScene* scene;

    // bounding box
    glm::vec3 AABBmin = glm::vec3(999, 999, 999);
    glm::vec3 AABBmax = glm:: vec3(-999, -999, -999);
    Cube* boundingBox;
    bool drawBoundingBox = true;

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
    Model(GameObject* owner);
    Model(Model* other);
    Model(GameObject* owner, std::string path, bool hasAnimation);
    void draw(const glm::mat4& viewProjMtx, GLuint shader);
    int getBoneCount() const;
    const aiScene* getScene() const;
    std::map<std::string, BoneInfo>& getBoneInfoMap();
    void addBoneCount();
    void update(float dt);
    void setPosition(glm::vec3 pos);
    void setRotation(glm::vec3 rot);
    void setScale(glm::vec3 scale);
    void setDrawBoundingBox(bool draw);
    const glm::vec3& getAABBmin() const;
    const glm::vec3& getAABBmax() const;
    void setAABBmin(glm::vec3 min);
    void setAABBmax(glm::vec3 max);
    
    std::string ToString() override;
};