#include "camera_scene.h"

#include "shapes/cube.h"
#include "shapes/dynamic.h"
#include "engine.h"
#include "utils.h"
#include <glm/glm.hpp>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

#include <glad/glad.h>

namespace {

unsigned int texture_id;
unsigned int PBO;

}

void setup_pbo(const cv::Mat& frame) {

  const int width = frame.cols;
  const int height = frame.rows;

  const int size_bytes =
    frame.total() * frame.elemSize();

  std::cout << "Init Byte Size: " << size_bytes << std::endl;

  // Setup texture
  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

  // Setup PBO
  glGenBuffers(1, &PBO);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size_bytes, 0, GL_STREAM_DRAW);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

void update_buffer(GLubyte* dst, const cv::Mat& frame) {
  if(!dst)
    return;

  unsigned char* ptr = (unsigned char*)dst;

  // height - rows
  // width. - cols
  int count = 0;
  for(int row = 0; row < frame.rows; ++row) {
    for(int col = 0; col < frame.cols; ++col) {
      const cv::Vec3b pix = frame.at<cv::Vec3b>(row, col);

      unsigned char r = pix[2];
      unsigned char g = pix[1];
      unsigned char b = pix[0];

      (*ptr) = r;
      ++ptr;
      ++count;
      (*ptr) = g;
      ++ptr;
      ++count;
      (*ptr) = b;
      ++ptr;
      ++count;

    }
  }

  // std::cout << "Count: " << count << std::endl;

}

void update_pbo(const cv::Mat& frame) {
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);

  const int width = frame.cols;
  const int height = frame.rows;
  const int size_bytes =
    frame.total() * frame.elemSize();
  // glTexSubImage2D

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size_bytes, 0, GL_STREAM_DRAW);

  GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  if(ptr) {
    // std::cout << "Updating buffer" << std::endl;
    update_buffer(ptr, frame);
    glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
  }

  // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RBG, GL_UNSIGNED_BYTE, 0);
  glTexSubImage2D(GL_TEXTURE_2D, texture_id, 0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, 0);

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

}

CameraScene::CameraScene(
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

  // int top_x = -5;
  // int top_y = 2;
  // m_height = 200;
  // m_width = 280;
  // double scale = 0.05;

  // for(int h = 0; h < m_height; ++h) {
  //   std::vector<std::shared_ptr<Cube>> row;

  //   for(int r = 0; r < m_width; ++r) {

  //     double x = top_x + (r * scale);
  //     double y = top_y - (h * scale);

  //     const glm::vec3 pos(x, y, -10.0f);
  //     Color color = {.r=255, .g=255, .b=255};

  //     auto pixel = std::make_shared<Cube>(pos, color);
  //     pixel->Scale(scale);

  //     row.push_back(pixel);
  //   }

  //   m_grid.push_back(row);

  // }

  const glm::vec3 pos(-1.0f, 0.0f, -10.0f);
  Color color = {.r=255, .g=255, .b=255};
  m_canvas = std::make_shared<Cube>(pos);
  m_canvas->ScaleX(4.0f);
  m_canvas->ScaleY(3.0f);

  directional_light().set_direction(glm::vec3(-0.2f, -1.0f, -0.3f));
  directional_light().set_ambient(glm::vec3(0.5f, 0.5f, 0.5f));
  directional_light().set_diffuse(glm::vec3(0.1f, 0.1f, 0.1f));
  directional_light().set_specular(glm::vec3(0.5f, 0.5f, 0.5f));

}

void CameraScene::render() {

  m_video >> m_frame;

  if(m_frame.empty()) {
    std::cout << "Empty frame" << std::endl;
    return;
  }

  if(!m_init) {
    setup_pbo(m_frame);
    m_init = true;
  }

  update_pbo(m_frame);

  m_canvas->SetTexture(texture_id);
  m_canvas->render();

  // std::cout << m_frame.elemSize() * m_frame.total() << std::endl;
  // std::cout << sizeof(unsigned char) << std::endl;

  // rows height, cols width
  // int start_width = (m_camera_width - m_width) / 2;
  // int start_height = (m_camera_height - m_height) / 2;

  // for(int i = 0; i < m_height; ++i) {
  //   for(int j = 0; j < m_width; ++j) {

  //     int frame_x = m_frame.cols - start_width - j;
  //     int frame_y = start_height + i;
  //     cv::Vec3b pix = m_frame.at<cv::Vec3b>(frame_y, frame_x);

  //     unsigned char r = pix[2];
  //     unsigned char g = pix[1];
  //     unsigned char b = pix[0];

  //     const Color c = {
  //       .r = r,
  //       .g = g,
  //       .b = b,
  //     };
      
  //     auto pixel = m_grid[i][j];
  //     pixel->SetColor(c);
  //     pixel->render();
  //   }
  // }
}

void CameraScene::controls() {

}
