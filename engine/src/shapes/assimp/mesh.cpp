#include "shapes/assimp/mesh.h"

#include <vector>

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

void Mesh::Render(Shader& shader)
{
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  for(unsigned int i = 0; i < m_textures.size(); ++i)
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