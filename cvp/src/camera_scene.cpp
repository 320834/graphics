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
unsigned int PBO[2];
GLenum error;

}

namespace CM {
int index = 0;
}

void print_error(GLenum error) {
  switch (error) {
    case GL_NO_ERROR:
        std::cout << "GL_NO_ERROR: No error has been recorded.\n";
        break;

    case GL_INVALID_ENUM:
        std::cout << "GL_INVALID_ENUM: An unacceptable value is specified for an enumerated argument.\n";
        break;

    case GL_INVALID_VALUE:
        std::cout << "GL_INVALID_VALUE: A numeric argument is out of range.\n";
        break;

    case GL_INVALID_OPERATION:
        std::cout << "GL_INVALID_OPERATION: The specified operation is not allowed in the current state.\n";
        break;

    case GL_INVALID_FRAMEBUFFER_OPERATION:
        std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION: The framebuffer object is not complete.\n";
        break;

    case GL_OUT_OF_MEMORY:
        std::cout << "GL_OUT_OF_MEMORY: There is not enough memory left to execute the command.\n";
        break;

    case GL_STACK_UNDERFLOW:
        std::cout << "GL_STACK_UNDERFLOW: An operation would cause an internal stack to underflow.\n";
        break;

    case GL_STACK_OVERFLOW:
        std::cout << "GL_STACK_OVERFLOW: An operation would cause an internal stack to overflow.\n";
        break;

    default:
        std::cout << "Unknown OpenGL error: 0x" << std::hex << error << std::dec << '\n';
        break;
  }
}

void setup_pbo(const cv::Mat& frame) {

  const int width = frame.cols;
  const int height = frame.rows;

  std::cout << "Width: " << width << " Height: " << height << std::endl;
  std::cout << "Element Size: " << frame.elemSize() << std::endl;
  std::cout << "GL_UNSIGNED_BYTE: " << sizeof(GL_UNSIGNED_BYTE) << std::endl;
  std::cout << "GLubyte: " << sizeof(GLubyte) << std::endl;

  const int size_bytes =
    frame.total() * (frame.elemSize() + 1);

  std::cout << "Init Byte Size: " << size_bytes << std::endl;

  // Setup texture
  glGenTextures(1, &texture_id);
  glActiveTexture(GL_TEXTURE0 + texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  // Fiddle around with these values
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (void*)nullptr);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Init PBO (GlTexImage2D): ";
    print_error(error);
  }

  // Setup PBO
  glGenBuffers(1, &PBO[0]);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO[0]);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size_bytes, nullptr, GL_STREAM_DRAW);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Init PBO 1 (GlBufferData): ";
    print_error(error);
  }

  glGenBuffers(1, &PBO[1]);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO[1]);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size_bytes, nullptr, GL_STREAM_DRAW);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Init PBO 2 (GlBufferData): ";
    print_error(error);
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

}

void update_buffer(void* dst, const cv::Mat& frame) {
  if(!dst)
    return;

  GLubyte* ptr = (GLubyte*)dst;

  // height - rows
  // width. - cols
  for(int row = 0; row < frame.rows; ++row) {
    for(int col = 0; col < frame.cols; ++col) {
      const cv::Vec3b pix = frame.at<cv::Vec3b>(row, col);

      int r = pix[2];
      int g = pix[1];
      int b = pix[0];

      (*ptr) = r;
      ++ptr;
      (*ptr) = g;
      ++ptr;
      (*ptr) = b;
      ++ptr;
      (*ptr) = 255;
      ++ptr;

    }
  }

}

void update_pbo(const cv::Mat& frame) {

  const int width = frame.cols;
  const int height = frame.rows;
  const int size_bytes =
    frame.total() * (frame.elemSize() + 1);

  // std::cout << "Size Bytes: " << size_bytes << std::endl;

  CM::index = (CM::index + 1) % 2;
  int nextIndex = (CM::index + 1) % 2;

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO[CM::index]);


  // glActiveTexture(GL_TEXTURE0);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)nullptr);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Update PBO (GlTexSubImage): ";
    print_error(error);
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO[nextIndex]);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size_bytes, 0, GL_STREAM_DRAW);
  // GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
  void* ptr = glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size_bytes, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Update PBO (GlMapBufferRange): ";
    print_error(error);
  }
  
  if(ptr) {
    // std::cout << "Updating buffer" << std::endl;
    update_buffer(ptr, frame);
    // glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    if(!glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER)) {
      std::cout << "glUnmapBuffer failed" << std::endl;
    }
  }


  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

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

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Pixel Store: ";
    print_error(error);
  }

  m_video >> m_frame;
  if(m_frame.empty()) {
    throw std::runtime_error("Frame empty");
  }

  setup_pbo(m_frame);
  update_pbo(m_frame);

  const glm::vec3 pos(-1.0f, 0.0f, -10.0f);
  Color color = {.r=255, .g=255, .b=255};
  m_canvas = std::make_shared<Cube>(pos);
  m_canvas->ScaleX(4.0f);
  m_canvas->ScaleY(3.0f);

  // glActiveTexture(GL_TEXTURE0);
  m_canvas->SetTexture(texture_id);
  // m_canvas->SetTexture("../shaders/container.jpg");

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

  update_pbo(m_frame);

  glActiveTexture(GL_TEXTURE0 + texture_id);
  m_canvas->render();

}

void CameraScene::controls() {

}
