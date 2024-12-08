#ifndef SQUARE_H_
#define SQUARE_H_

#include "shape.h"

#include <glm/glm.hpp>

class Square : public Shape {
public:
  Square();
  Square(const glm::vec3& position);

  Square(const Square& other);
  Square& operator=(const Square& other);

  void render() override;

  static unsigned int m_VAO;
  static unsigned int m_VBO;
  static unsigned int m_stride;
  static std::vector<VertexType> m_vertices;
  static void init_vertex_buffers();
};

#endif //SQUARE_H_
