#ifndef CUBE_H_
#define CUBE_H_

#include "shape.h"

#include <glm/glm.hpp>

struct Color {
  unsigned char r;
  unsigned char g;
  unsigned char b;
};

inline const Color DEFAULT_COLOR = {
  .r = 255,
  .g = 255,
  .b = 255
};

class Cube : public Shape {
public:

  Cube(const glm::vec3& position);
  Cube(
    const glm::vec3& position,
    const std::string& texture_name
  );
  Cube(
    const glm::vec3& position,
    const Color color
  );

  const int TextureId() const;
  const Color GetColor() const;

  void SetTexture(const std::string& texture_name);
  void SetColor(const Color color);

  Cube(const Cube& other);
  Cube& operator=(const Cube& other);

  void render() override;

  static unsigned int m_VAO;
  static unsigned int m_VBO;
  static unsigned int m_stride;
  static std::vector<VertexType> m_vertices;
  static void init_vertex_buffers();

protected:

  void use_texture();
  
  int m_texture_id;
  struct Color m_color;
  bool m_use_color;

};

#endif //CUBE_H_
