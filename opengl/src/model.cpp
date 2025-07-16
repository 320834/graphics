#include "model.h"

#include "image/stb_image.h"

unsigned int texture_from_file(
  const std::string& path,
  const std::string& directory)
{
  const std::string filename =
    directory + "/" + path;
  unsigned int textureID;
  glGenTextures(1, &textureID);
  
  int width, height, nrComponents;
  unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
  if (data) {
    GLenum format;
    if (nrComponents == 1) {
      format = GL_RED;
    }
    else if (nrComponents == 3) {
      format = GL_RGB;
    }
    else if (nrComponents == 4) {
      format = GL_RGBA;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);
  }
  else
  {
    std::cout << "Texture failed to load at path: " << path << std::endl;
    stbi_image_free(data);
  }

  return textureID;
}

Mesh::Mesh(
  const std::vector<Vertex>& vertices, 
  const std::vector<unsigned int>& indices,
  const std::vector<Texture>& textures
) 
{
  m_vertices = vertices;
  m_indices = indices;
  m_textures = textures;

  setup_mesh();
}

void Mesh::setup_mesh() {
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  glBindVertexArray(m_VAO);

  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(
    GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex),
    &m_vertices[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() *
                sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

  // vertex positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, Normal));
  // vertex texture coords
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void*)offsetof(Vertex, TexCoords));
  glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for(unsigned int i = 0; i < m_textures.size(); i++)
  {
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = m_textures[i].type;
    if(name == "texture_diffuse") {
      number = std::to_string(diffuseNr++);
    }
    else if(name == "texture_specular") {
      number = std::to_string(specularNr++);
    }

    // shader.setFloat(("material." + name + number).c_str(), i);
    const std::string uniform = "material." + (name + number);
    shader.setInt(uniform, i);

    shader.setFloat("material.shininess", 64.0f);

    glActiveTexture(GL_TEXTURE0 + i); // activate texture unit first
    glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
  }

  // draw mesh
  glBindVertexArray(m_VAO);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  glActiveTexture(GL_TEXTURE0);
}

Model::Model(const std::string& path)
{
  load_model(path);
}

void Model::load_model(const std::string path) {
  Assimp::Importer import;

  const aiScene *scene = import.ReadFile(
    path, aiProcess_Triangulate | aiProcess_FlipUVs
  );

  if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
     !scene->mRootNode)
  {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    return;
  }

  m_directory = path.substr(0, path.find_last_of('/'));

  process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode *node, const aiScene *scene) {

  for(unsigned int i = 0; i < node->mNumMeshes; ++i) {

    // node->mMeshes stores the array index
    // of what it will be in scene
    aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
    Mesh mesh = process_mesh(ai_mesh, scene);

    m_meshes.push_back(mesh);
  }

  for(unsigned int i = 0; i < node->mNumChildren; ++i) {
    process_node(node->mChildren[i], scene);
  }
}

Mesh Model::process_mesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<Vertex> vertices;
  std::vector<unsigned int> indices;
  std::vector<Texture> textures;

  for(unsigned int i = 0; i < mesh->mNumVertices; ++i) {
    Vertex vertex;

    glm::vec3 vector;
    vector.x = mesh->mVertices[i].x;
    vector.y = mesh->mVertices[i].y;
    vector.z = mesh->mVertices[i].z;

    vertex.Position = vector;

    glm::vec3 normal;
    normal.x = mesh->mNormals[i].x;
    normal.y = mesh->mNormals[i].y;
    normal.z = mesh->mNormals[i].z;

    vertex.Normal = normal;

    if(mesh->mTextureCoords[0]) {
      glm::vec2 tex_coords;
      tex_coords.x =
        mesh->mTextureCoords[0][i].x;
      
      tex_coords.y =
        mesh->mTextureCoords[0][i].y;
    
      vertex.TexCoords = tex_coords;
    } else {
      vertex.TexCoords = glm::vec2(0.0f, 0.0f);
    }

    vertices.push_back(vertex);
  }

  for(unsigned int i = 0; i < mesh->mNumFaces; ++i) {
    aiFace face = mesh->mFaces[i];

    for(unsigned int j = 0; j < face.mNumIndices; ++j) {
      indices.push_back(face.mIndices[j]);
    }
  }

  if(mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps =
      load_material_textures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = 
      load_material_textures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
  } else {
    std::cout << "Missing vertex attributes" << std::endl;
  }

  return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::load_material_textures(
  aiMaterial *mat, aiTextureType type, std::string typeName
) {

  // if(!mat) {
  //   return {};
  // }

  std::vector<Texture> textures;

  for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
  {
    aiString filepath;
    mat->GetTexture(type, i, &filepath);

    const auto iter = std::find_if(
      m_textures_loaded.begin(),
      m_textures_loaded.end(),
      [filepath](const Texture& tex) -> bool {        
        return std::strcmp(tex.path.data(), filepath.C_Str()) == 0;
      }
    );

    if(iter == m_textures_loaded.end()) {
      Texture texture;
      texture.id = texture_from_file(filepath.C_Str(), m_directory);
      texture.type = typeName;
      texture.path = filepath.C_Str();
      textures.push_back(texture);
      m_textures_loaded.push_back(texture);
    } else {
      textures.push_back(*iter);
    }
  }

  return textures;
}

void Model::Draw(Shader& shader) {
  for(unsigned int i = 0; i < m_meshes.size(); ++i) {
    m_meshes[i].Draw(shader);
  }
}