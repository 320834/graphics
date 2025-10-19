#ifndef NAIVE_CAMERA_SCENE_H_
#define NAIVE_CAMERA_SCENE_H_

#include "scene.h"
#include "shapes/cube.h"
#include "shapes/shape.h"
#include "opengl_wrapper.h"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

using Grid = std::vector<std::vector<std::shared_ptr<Cube>>>;

class NaiveCameraScene : public SceneInterface {
public:
  NaiveCameraScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
  );
  ~NaiveCameraScene() = default;

  void render() override;
  void controls() override;

  Grid m_grid;
  cv::VideoCapture m_video;
  cv::Mat m_frame;

  int m_width;
  int m_height;

  int m_camera_width;
  int m_camera_height;
};

#endif //NAIVE_CAMERA_SCENE_H_