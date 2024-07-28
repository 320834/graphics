#ifndef EXPERIMENT_CUBE_H_
#define EXPERIMENT_CUBE_H_

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "image/stb_image.h"
#include "experiment/utils.h"

struct TextureLoadData {
  int width;
  int height;
  int nr_channels;
  unsigned char* data;
};

struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

const std::string EMPTY;

namespace cube {
const int g_cube_stride = 5;
const float vertices_cube[] = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // TopBackLeft
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f, // BottomFrontRight
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

inline bool is_overlapping(
  const glm::vec3& bbl,
  const glm::vec3& tfr,
  const glm::vec3& corner
)
{

  const bool isX = bbl.x <= corner.x && corner.x <= tfr.x;
  const bool isY = bbl.y <= corner.y && corner.y <= tfr.y;
  const bool isZ = bbl.z <= corner.z && corner.z <= tfr.z;

  return isX && isY && isZ;
}

class Cube {

public:

  struct Collision {
    bool collide = false;
    size_t points = 0;
  };

  struct Scale {
    size_t x = 0;
    size_t y = 0;
    size_t z = 0;
  };

  Cube(const unsigned int shader_id);
  Cube(
    const unsigned int shader_id,
    const glm::vec3 position
  );
  Cube(
    const unsigned int shader_id,
    const glm::vec3 position,
    const std::string& texture_name
  );

  Cube(
    const unsigned int shader_int,
    const glm::vec3 position,
    const Color color
  );

  Cube(const Cube& other);
  Cube& operator=(const Cube& other);

  const unsigned ShaderId() const;
  const int TextureId() const;
  const glm::mat4 TransformMatrix() const;
  const glm::mat4 ScaleMatrix() const;
  const glm::mat4 RotateMatrix() const;

  void Render();
  void SetPosition(const glm::vec3& position);
  void SetScale(const float scale);

  void SetTexture(const std::string& texture_name);
  void SetColor(const Color color);

  void Transform(const glm::vec3& position);
  void Scale(const float scale);
  void ScaleX(const float scale);
  void ScaleY(const float scale);
  void ScaleZ(const float scale);

  std::vector<glm::vec3> GetCorners() const;
  glm::vec3 GetPosition() const;
  glm::vec3 BottomBackLeft() const;
  glm::vec3 TopFrontRight() const;

  Collision IsColliding(const Cube& cube) const;

private:

  void LoadTexture(
    TextureLoadData& texture_data,
    const std::string extension
  );

  void FailLoadTexture();

  int m_texture_id;
  glm::mat4 m_transformation;
  glm::mat4 m_rotation;
  glm::mat4 m_scale;
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
  m_transformation = glm::mat4(1.0f);
  m_rotation = glm::mat4(1.0f);
  m_scale = glm::mat4(1.0f);
  m_texture_id = -1;

  Transform(position);

  SetTexture(texture_name);
}

inline Cube::Cube(
  const unsigned int shader_id,
  const glm::vec3 position,
  const Color color
)
  : Cube(shader_id, position, EMPTY)
{
  SetColor(color);
}


inline Cube::Cube(const Cube& other)
 : m_shader_id(other.ShaderId())
{
  m_texture_id = other.TextureId();
  m_transformation = other.TransformMatrix();
  m_scale = other.ScaleMatrix();
  m_rotation = other.RotateMatrix();
}

inline Cube& Cube::operator=(const Cube& other) {
  if(this == &other) {
    return *this;
  }

  m_texture_id = other.TextureId();
  m_transformation = other.TransformMatrix();
  m_scale = other.ScaleMatrix();
  m_rotation = other.RotateMatrix();

  return *this;
}

inline const unsigned Cube::ShaderId() const {
  return m_shader_id;
}

inline const int Cube::TextureId() const {
  return m_texture_id;
}

inline const glm::mat4 Cube::TransformMatrix() const {
  return m_transformation;
}

inline const glm::mat4 Cube::RotateMatrix() const {
  return m_rotation;
}

inline const glm::mat4 Cube::ScaleMatrix() const {
  return m_scale;
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
  m_model = glm::mat4(1.0f);
  m_model = m_transformation * m_rotation * m_scale;

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(m_model));

  glDrawArrays(GL_TRIANGLES, 0, 36);
}

inline void Cube::SetPosition(const glm::vec3& position) {
  m_transformation = glm::mat4(1);
  m_transformation = glm::translate(m_transformation, position);
}

inline void Cube::Transform(const glm::vec3& position) {
  m_transformation = glm::translate(m_transformation, position);
}

inline void Cube::Scale(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(scale));
}

inline void Cube::ScaleX(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(scale, 1.0f, 1.0f));
}

inline void Cube::ScaleY(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(1.0f, scale, 1.0f));
}

inline void Cube::ScaleZ(const float scale) {
  m_scale = glm::scale(m_scale, glm::vec3(1.0f, 1.0f, scale));
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

inline void Cube::SetColor(const Color color) {
   unsigned int use_texture_id = glGetUniformLocation(m_shader_id, "use_texture");
   glUniform1i(use_texture_id, (int)false);

   unsigned int color_id = glGetUniformLocation(m_shader_id, "color");

   float r = ((float)color.r) / 255;
   float g = ((float)color.g) / 255;
   float b = ((float)color.b) / 255;
   glUniform4f(color_id, r, g, b, 1.0);
}

inline std::vector<glm::vec3> Cube::GetCorners() const {
  std::vector<glm::vec4> corners = {
    {-0.5f, -0.5f, -0.5f, 1.0f}, // bottom_back_left
    {0.5, -0.5, -0.5, 1.0f},
    {-0.5, 0.5, -0.5, 1.0f},
    {0.5, 0.5, -0.5f, 1.0f},

    {-0.5f, -0.5f, -0.5f, 1.0f},
    {0.5, -0.5, 0.5, 1.0f},
    {-0.5f, 0.5f, 0.5f, 1.0f},
    {0.5f, 0.5f, 0.5f, 1.0f} // top_front_right
  };

  std::vector<glm::vec3> ret_corners;
  for(auto& corner : corners) {
    glm::vec3 ret_corner =
      m_transformation * m_rotation * m_scale * corner;

    ret_corners.push_back(ret_corner);
  }

  return ret_corners;
}

inline glm::vec3 Cube::GetPosition() const {
  return glm::vec3(
    m_transformation[3][0],
    m_transformation[3][1],
    m_transformation[3][2]
  );
}

inline glm::vec3 Cube::BottomBackLeft() const {
  const glm::vec4 vertex = {-0.5f, -0.5f, -0.5f, 1.0f};

  return m_transformation * m_rotation * m_scale * vertex;
}

inline glm::vec3 Cube::TopFrontRight() const {
  const glm::vec4 vertex = {0.5f, 0.5f, 0.5f, 1.0f};

  return m_transformation * m_rotation * m_scale * vertex;
}

inline Cube::Collision Cube::IsColliding(const Cube& cube) const {

  size_t points = 0;
  bool found_collision = false;

  glm::vec3 bbl = BottomBackLeft();
  glm::vec3 tfr = TopFrontRight();

  const auto corners_other = cube.GetCorners();
  for(const glm::vec3& corner : corners_other) {
    // utils::print_glm_vec(corner);
    if(is_overlapping(bbl, tfr, corner)) {
      found_collision = true;
      ++points;
    }
  }

  if(found_collision) {
    return {
      .collide = found_collision,
      .points = points
    };
  }

  // Check other cube
  glm::vec3 bbl_other = cube.BottomBackLeft();
  glm::vec3 tfr_other = cube.TopFrontRight();

  const std::vector<glm::vec3> corners_curr = GetCorners();
  for(const glm::vec3& corner : corners_curr) {
    if(is_overlapping(bbl_other, tfr_other, corner)) {
      found_collision = true;
      ++points;
    }
  }

  return {
    .collide = found_collision,
    .points = points
  };

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
