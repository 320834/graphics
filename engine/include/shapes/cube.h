#ifndef CUBE_H_
#define CUBE_H_

#include "shape.h"

#include <glm/glm.hpp>

class Cube : public Shape {
public:

  Cube(const glm::vec3& position);

  Cube(const Cube& other);
  Cube& operator=(const Cube& other);

  void render() override;

  static unsigned int m_VAO;
  static unsigned int m_VBO;
  static unsigned int m_stride;
  static std::vector<VertexType> m_vertices;
  static void init_vertex_buffers();

};

#endif //CUBE_H_
