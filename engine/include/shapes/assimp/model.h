#ifndef SHAPES_ASSIMP_MODEL_H_
#define SHAPES_ASSIMP_MODEL_H_

#include <string>

#include "shapes/assimp/mesh.h"
#include "shader.h"

unsigned int texture_from_file(
  const std::string& path,
  const std::string& directory
);

class Model {
  public:
    Model(const std::string& path);
    void Render(Shader& shader);

  private:
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    std::vector<Texture> m_textures_loaded;
  
    bool load_model(const std::string path);
    void process_node(aiNode *node, const aiScene *scene);
    Mesh process_mesh(aiMesh *mesh, const aiScene *scene);
  
    std::vector<Texture> load_material_textures(
      aiMaterial *mat, aiTextureType type, std::string typeName
    );
  
};

#endif //SHAPES_ASSIMP_MODEL_H_
