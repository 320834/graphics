#ifndef SHAPES_ASSIMP_MESH_H
#define SHAPES_ASSIMP_MESH_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

#include "shader.h"

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
};

struct Texture {
  unsigned int id;
  std::string type;
  std::string path;
};

class Mesh {
  public:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
  
    Mesh(
      const std::vector<Vertex>& vertices,
      const std::vector<unsigned int>& indices,
      const std::vector<Texture>& textures
    );
  
    void Render(Shader& shader);
  
  private:
    unsigned int m_VBO, m_VAO, m_EBO;
  
    void setup_mesh();
};

#endif //SHAPES_ASSIMP_MESH_H
