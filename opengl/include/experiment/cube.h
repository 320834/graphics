#ifndef EXPERIMENT_CUBE_H_
#define EXPERIMENT_CUBE_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "image/stb_image.h"

struct TextureLoadData {
  int width;
  int height;
  int nr_channels;
  unsigned char* data;
};

const std::string EMPTY;

namespace cube {
const float vertices_cube[] = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};
} // namespace cube
class Cube {

public:
  Cube(const unsigned int shader_id);
  Cube(const unsigned int shader_id, const glm::vec3 position);
  Cube(
    const unsigned int shader_id,
    const glm::vec3 position,
    const std::string& texture_name
  );
  void Render();
  void SetPosition(const glm::vec3& position);
  void SetScale(const float scale);
  void SetTexture(const std::string& texture_name);

  void Transform(const glm::vec3& position);
  void Scale(const float scale);

  glm::vec3 GetPosition();

private:

  void LoadTexture(
    TextureLoadData& texture_data,
    const std::string extension
  );

  void FailLoadTexture();

  int m_texture_id;
  float m_scale = 1.0f;
  glm::vec3 m_position;
  glm::mat4 m_model;
  const unsigned m_shader_id;

};

inline Cube::Cube(const unsigned int shader_id) 
  : Cube(shader_id, glm::vec3(0.0f, 0.0f, 0.0f)) {}

inline Cube::Cube(const unsigned int shader_id, const glm::vec3 position)
  : Cube(shader_id, position, EMPTY)
{}

inline Cube::Cube(
  const unsigned int shader_id,
  const glm::vec3 position,
  const std::string& texture_name
) : m_shader_id(shader_id)
{
  m_model = glm::mat4(1.0f);
  m_position = position;
  m_texture_id = -1;

  Transform(position);

  SetTexture(texture_name);
}

inline void Cube::Render() {

  // Handle textures
  unsigned int use_texture_id = glGetUniformLocation(m_shader_id, "use_texture");
  if(m_texture_id == -1) {
    glUniform1i(use_texture_id, (int)false);
  } else {
    glUniform1i(use_texture_id, (int)true);

    // unsigned int texture_uni = glGetUniformLocation(m_shader_id, "texture_one");
    // glUniform1i(texture_uni, m_texture_id);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texture_id);    
  }

  unsigned int model_id = glGetUniformLocation(m_shader_id, "model");

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));

  glDrawArrays(GL_TRIANGLES, 0, 36);
}

inline void Cube::SetPosition(const glm::vec3& position) {
  m_model = glm::mat4(1);
  m_model = glm::translate(m_model, position);
}

inline void Cube::Transform(const glm::vec3& position) {
  m_model = glm::translate(m_model, position);
}

inline void Cube::Scale(const float scale) {
  m_model = glm::scale(m_model, glm::vec3(scale));
}

inline void Cube::SetTexture(const std::string& texture_name) {

  if(texture_name.empty()) {
    FailLoadTexture();
    return;
  }

  const std::string path = "../textures/" + texture_name;
  int width, height, nr_channels;
  unsigned char *data = stbi_load(path.c_str(), &width, &height,
    &nr_channels, 0);

  // null that means something bad happened
  if(!data) {
    FailLoadTexture();
    return;
  }

  auto pos = texture_name.find(".");
  if(pos == std::string::npos) {
    FailLoadTexture();
    return;
  }

  const std::string extension =
    texture_name.substr(pos);

  TextureLoadData texture_data = {
    .width = width,
    .height = height,
    .nr_channels = nr_channels,
    .data = data
  };

  LoadTexture(texture_data, extension);
}

inline glm::vec3 Cube::GetPosition() {
  return m_position;
}

inline void Cube::LoadTexture(
  TextureLoadData& texture_data,
  const std::string extension
) {

  unsigned int texture;

  // Check if texture is already set
  if(m_texture_id == -1) {
    glGenTextures(1, &texture);
  } else {
    texture = m_texture_id;
  }
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // set texture filtering parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  if(auto search = extension.find(".png"); search != std::string::npos) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGBA,
      GL_UNSIGNED_BYTE, texture_data.data);
  } else {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_data.width, texture_data.height, 0, GL_RGB,
      GL_UNSIGNED_BYTE, texture_data.data);
  }

  glGenerateMipmap(GL_TEXTURE_2D);
  m_texture_id = texture;
  unsigned int use_texture_id = glGetUniformLocation(m_shader_id, "use_texture");
  glUniform1i(use_texture_id, (int)true);


  stbi_image_free(texture_data.data);
}

inline void Cube::FailLoadTexture() {
  unsigned int use_texture_id = glGetUniformLocation(m_shader_id, "use_texture");
  glUniform1i(use_texture_id, (int)false);
  m_texture_id = -1;
}

#endif //EXPERIMENT_CUBE_H_
