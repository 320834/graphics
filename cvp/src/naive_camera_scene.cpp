#include "naive_camera_scene.h"

#include "shapes/cube.h"
#include "shapes/dynamic.h"
#include "engine.h"
#include "utils.h"
#include <glm/glm.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

NaiveCameraScene::NaiveCameraScene(
    const std::shared_ptr<Engine<OpenGLWrapper>>& engine,
    const std::string& scene_name
)
  : SceneInterface(engine, scene_name)
{

  m_camera_height = 480;
  m_camera_width = 640;
  m_video.open(0);
  m_video.set(cv::CAP_PROP_FRAME_WIDTH, m_camera_width);
  m_video.set(cv::CAP_PROP_FRAME_HEIGHT, m_camera_height);

  if(!m_video.isOpened()) {
    throw std::runtime_error("Cannot open camera");
  }

  m_engine->set_background({0.2f, 0.2f, 0.2f});

  int top_x = -5;
  int top_y = 2;
  m_height = 200;
  m_width = 200;
  double scale = 0.05;

  for(int h = 0; h < m_height; ++h) {
    std::vector<std::shared_ptr<Cube>> row;

    for(int r = 0; r < m_width; ++r) {

      double x = top_x + (r * scale);
      double y = top_y - (h * scale);

      const glm::vec3 pos(x, y, -10.0f);
      Color color = {.r=255, .g=255, .b=255};

      auto pixel = std::make_shared<Cube>(pos, color);
      pixel->Scale(scale);

      row.push_back(pixel);
    }

    m_grid.push_back(row);

  }

  directional_light().set_direction(glm::vec3(-0.2f, -1.0f, -0.3f));
  directional_light().set_ambient(glm::vec3(0.5f, 0.5f, 0.5f));
  directional_light().set_diffuse(glm::vec3(0.1f, 0.1f, 0.1f));
  directional_light().set_specular(glm::vec3(0.5f, 0.5f, 0.5f));

}

void NaiveCameraScene::render() {

  m_video >> m_frame;

  if(m_frame.empty()) {
    std::cout << "Empty frame" << std::endl;
    return;
  }

  // rows height, cols width
  int start_width = (m_camera_width - m_width) / 2;
  int start_height = (m_camera_height - m_height) / 2;

  for(int i = 0; i < m_height; ++i) {
    for(int j = 0; j < m_width; ++j) {

      int frame_x = m_frame.cols - start_width - j;
      int frame_y = start_height + i;
      cv::Vec3b pix = m_frame.at<cv::Vec3b>(frame_y, frame_x);

      unsigned char r = pix[2];
      unsigned char g = pix[1];
      unsigned char b = pix[0];

      const Color c = {
        .r = r,
        .g = g,
        .b = b,
      };
      
      auto pixel = m_grid[i][j];
      pixel->SetColor(c);
      pixel->render();
    }
  }
}

void NaiveCameraScene::controls() {

}