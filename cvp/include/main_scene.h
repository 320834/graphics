#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_

#include "scene.h"
#include "shapes/shape.h"
#include "opengl_wrapper.h"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class MainScene : public SceneInterface {
public:
  MainScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
  );
  ~MainScene() = default;

  void render() override;
  void controls() override;

  std::vector<std::shared_ptr<Shape>> m_shapes;
  cv::VideoCapture m_video;
  cv::Mat m_frame;
};

#endif //MAIN_SCENE_H_