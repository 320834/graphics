#ifndef MAIN_SCENE_H_
#define MAIN_SCENE_H_

#include "scene.h"
#include "shapes/cube.h"
#include "shapes/shape.h"
#include "opengl_wrapper.h"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using Grid = std::vector<std::vector<std::shared_ptr<Cube>>>;

class CameraScene : public SceneInterface {
public:
  CameraScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
  );
  ~CameraScene() = default;

  void render() override;
  void controls() override;

  Grid m_grid;
  cv::VideoCapture m_video;
  cv::Mat m_frame;

  int m_width;
  int m_height;

  int m_camera_width;
  int m_camera_height;

  bool m_init = false;

  std::shared_ptr<Cube> m_canvas;

};

#endif //MAIN_SCENE_H_
