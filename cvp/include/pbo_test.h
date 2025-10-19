#ifndef PBO_TEST_SCENE_H_
#define PBO_TEST_SCENE_H_

#include "scene.h"
#include "shapes/cube.h"
#include "shapes/shape.h"
#include "opengl_wrapper.h"

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

class PboTestScene : public SceneInterface {
public:
  PboTestScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
  );

  ~PboTestScene() = default;

  void render() override;
  void controls() override;

  void setup_pbo(const cv::Mat& frame);
  void update_pbo();

  cv::VideoCapture m_video;
  cv::Mat m_frame;

  int m_width;
  int m_height;

  int m_camera_width;
  int m_camera_height;

  std::shared_ptr<Cube> m_canvas;

  int m_pbo_index;
  unsigned int m_texture_id;
  unsigned int m_PBO[2];

  GLenum m_error;

private:
  void update_buffer(void* dst);
  void print_error();

};

#endif //PBO_TEST_SCENE_H_
