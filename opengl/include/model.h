#ifndef MODEL_H_
#define MODEL_H_

#include <glm/glm.hpp>
#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"

unsigned int texture_from_file(
  const std::string& path,
  const std::string& directory
);

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

  void Draw(Shader& shader);

private:
  unsigned int m_VBO, m_VAO, m_EBO;

  void setup_mesh();
};

class Model {
public:
  Model(const std::string& path);
  void Draw(Shader& shader);
private:
  std::vector<Mesh> m_meshes;
  std::string m_directory;
  std::vector<Texture> m_textures_loaded;

  void load_model(const std::string path);
  void process_node(aiNode *node, const aiScene *scene);
  Mesh process_mesh(aiMesh *mesh, const aiScene *scene);

  std::vector<Texture> load_material_textures(
    aiMaterial *mat, aiTextureType type, std::string typeName
  );

};

#endif //MODEL_H_
