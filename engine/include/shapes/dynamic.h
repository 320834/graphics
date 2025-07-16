#ifndef SHAPES_DYNAMIC_H_
#define SHAPES_DYNAMIC_H_

#include <unordered_map>
#include <optional>
#include <string>

#include "shapes/shape.h"

struct DynamicShapeMetadata {
  unsigned int m_VAO;
  unsigned int m_VBO;
  unsigned int m_stride;
  std::vector<VertexType> m_vertices;
};

class DynamicShapeManager {
public:
  static bool load_shape(
    const std::string& dyn_shape_name,
    const std::string& filename
  );

  static std::optional<DynamicShapeMetadata> get_dyn_shape(
    const std::string& dyn_shape_name
  );

private:

  static std::unordered_map<std::string, DynamicShapeMetadata>
    m_dynamic_shapes;
};

class Dynamic : public Shape {
public:
  Dynamic(
    const std::string& name
  );
  Dynamic(
    const std::string& name,
    const glm::vec3& position
  );

  Dynamic(const Dynamic& other);
  Dynamic& operator=(const Dynamic& other);

  unsigned VAO() const;
  unsigned VBO() const;

  void render() override;
private:
  unsigned m_VAO;
  unsigned m_VBO;
};

#endif //SHAPES_DYNAMIC_H_
