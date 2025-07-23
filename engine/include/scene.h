#ifndef ENGINE_SCENE_H_
#define ENGINE_SCENE_H_

#include <vector>
#include <memory>
#include <string>
#include "opengl_wrapper.h"
#include "lights/directional.h"
#include "lights/point.h"
#include "lights/spot.h"

// Foward declare
template<class OpenGLWrapper>
class Engine;

class SceneInterface {
public:
  SceneInterface(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
  )
    : m_engine{engine},
      m_scene_name{scene_name}
  {}

  std::string scene_name() const;

  void set_lights();
  virtual void base_render() final;
  
  DirectionalLight& directional_light();
  std::shared_ptr<PointLight> add_point_light(const glm::vec3 position);
  std::shared_ptr<SpotLight> add_spot_light(const glm::vec3 position);

  virtual void render() = 0;
  virtual void controls() = 0;

protected:
  std::shared_ptr<Engine<OpenGLWrapper>> m_engine;
  DirectionalLight m_directional_light;
  std::vector<std::shared_ptr<PointLight>> m_point_lights;
  std::vector<std::shared_ptr<SpotLight>> m_spot_lights;
  std::string m_scene_name; // unique identifier
};

#endif // ENGINE_SCENE_H_
