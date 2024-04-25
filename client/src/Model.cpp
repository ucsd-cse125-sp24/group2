#include "Model.h"

Model::Model() {

}

Model::Model(std::string path) {
    loadModel(path);
}

void Model::draw(const glm::mat4& viewProjMtx, GLuint shader) {
    for(int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(viewProjMtx, shader);
    }
}

void Model::loadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    } else {
        std::cout << "SUCCESS::ASSIMP:: LOADING FILE " << std::endl;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene) {
    for(int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for(int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    // setting vertices
    for(int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        // std::cout<<"position: "<<vertex.position.x<<" "<<vertex.position.y<<" "<<vertex.position.z<<std::endl;
        if(mesh->mNormals) {
            vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        }

        if(mesh->mTextureCoords[0]) {
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            vertex.texCoords = glm::vec2(0.0f, 0.0f);
        }
        vertices.push_back(vertex);
    }

    // setting faces
    for(int i = 0; i < mesh->mNumFaces; i++){
        aiFace face = mesh->mFaces[i];
        for(int j = 0; j < face.mNumIndices; j++){
            indices.push_back(face.mIndices[j]);
        }
    }

    // setting materials
    if(mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // 1. diffuse maps/ base colors
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. metallicRoughness maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_UNKNOWN, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    }
    return Mesh(vertices, indices, textures);

}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);
        // std::cout<<"texture name: "<< str.C_Str()<<std::endl;
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
        bool skip = false;
        for(unsigned int j = 0; j < textures_loaded.size(); j++) {
            if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                textures.push_back(textures_loaded[j]);
                skip = true; // a texture with the same filepath has already been loaded, continue to next one. (optimization)
                break;
            }
        }
        if(!skip) {   // if texture hasn't been loaded already, load it
            Texture texture;
            texture.id = textureFromFile(str.C_Str(), this->directory);
            // std::cout<<"texture id: "<<texture.id<<std::endl;
            texture.type = typeName;
            std::cout<<"typename: "<< typeName<<std::endl; 
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecessary load duplicate textures.
        }
    }
    return textures;
}

unsigned int Model::textureFromFile(const char *path, const std::string &directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;
    std::cout<<"filename: "<<filename<<std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {   
        // std::cout<<"format: "<<nrComponents<<std::endl;
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    } else {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }
    // std::cout<<"texture id: "<<textureID<<std::endl;
    return textureID;
}
