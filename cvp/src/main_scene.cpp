#include "main_scene.h"

#include "shapes/cube.h"
#include "shapes/dynamic.h"
#include "engine.h"
#include "utils.h"
#include <glm/glm.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

MainScene::MainScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
)
  : SceneInterface(engine, scene_name)
{

  m_video.open(0);
  m_video.set(cv::CAP_PROP_FRAME_WIDTH, 720);
  m_video.set(cv::CAP_PROP_FRAME_HEIGHT, 360);
  // m_video.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));

  if(!m_video.isOpened()) {
    throw std::runtime_error("Cannot open camera");
  }

  m_engine->set_background({0.2f, 0.2f, 0.2f});

  m_shapes.push_back(
    std::make_shared<Dynamic>(
      "backpack",
      glm::vec3(0.0f, 0.0f, -10.0f)
    )
  );

  directional_light().set_direction(glm::vec3(-0.2f, -1.0f, -0.3f));
  directional_light().set_ambient(glm::vec3(0.5f, 0.5f, 0.5f));
  directional_light().set_diffuse(glm::vec3(0.1f, 0.1f, 0.1f));
  directional_light().set_specular(glm::vec3(0.5f, 0.5f, 0.5f));

}

void MainScene::render() {

  for(std::shared_ptr<Shape>& shape : m_shapes) {
    shape->render();
  }

  m_video >> m_frame;

  if(m_frame.empty()) {
    std::cout << "Empty frame" << std::endl;
    return;
  }

  cv::imshow("Window", m_frame);
}

void MainScene::controls() {

}
