#ifndef SHAPES_DYNAMIC_H_
#define SHAPES_DYNAMIC_H_

#include <unordered_map>
#include <optional>
#include <string>
#include <memory>

#include "shapes/shape.h"
#include "shapes/assimp/model.h"

struct DynamicShapeMetadata {
  std::string name;
  std::string filename;
  std::shared_ptr<Model> model;
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

  void render() override;

private:

  std::string m_model_name;
  std::shared_ptr<Model> m_model;
};

#endif //SHAPES_DYNAMIC_H_
