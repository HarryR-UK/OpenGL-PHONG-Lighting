#ifndef MODEL_H
#define MODEL_H

#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Vertex.h"


#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


typedef glm::vec3 vec3;
typedef glm::vec2 vec2;

class Model
{
    private:
        std::vector<Mesh> m_meshes;
        std::string m_directory;
        std::vector<Texture> m_texturesLoaded;


    private:
        void loadModel(std::string path);
        void processNode(aiNode* node, const aiScene* scene);
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,std::string typeName);
        unsigned int textureFromFile(const char* path, const std::string& directory);
    public:

    public:
        Model(char* path);
        void Draw(Shader& shader);


};

#endif //!MODEL_H