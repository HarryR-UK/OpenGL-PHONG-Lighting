#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>
#include <vector>

#include "Shader.h"
#include "Texture.h"
#include "Vertex.h"

class Mesh
{
    private:
        unsigned int VAO,VBO, EBO;
        void setUpMesh();

    private:

    public:
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

    public:
        Mesh(std::vector<Vertex>vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Shader& shader);
};

#endif //!MESH_H