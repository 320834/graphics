#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "stb_image.h"
#include "callbacks.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

struct InitReturn {
  int status;
  GLFWwindow* window;
};

float vertices[] = {
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
  1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top-right

  // Second triangle
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
    1.0f,  1.0f, 0.0f, 1.0f, 1.0f, // top-right
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f  // top-left
};

float g_lastX = 400;
float g_lastY = 300;
bool g_isFirst = true;
float g_yaw = -90.0f;
float g_pitch = 0.0f;
float g_zoom = 45.0f;

// Something here
namespace {

const int camera_width = 200;
const int camera_height = 200;

unsigned int PBO;
unsigned int texture_id;

GLenum error;

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

}

InitReturn init(const std::string window_name) {
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  #ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  #endif

  GLFWwindow* window = glfwCreateWindow(800, 600, window_name.c_str(), NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return {
      .status=-1,
      .window=NULL
    };
  }
  glfwMakeContextCurrent(window);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    return {
      .status=-1,
      .window=NULL
    };
  }

  int viewport_width = 800;
  int viewport_height = 600;
  glfwGetFramebufferSize(window, &viewport_width, &viewport_height);
  glViewport(0, 0, viewport_width, viewport_height);

  return {
    .status = 0,
    .window = window
  };
}

void load_textures(int widths[1], int heights[1], int nrChannels[1], unsigned int textures[1]) {
  {
    unsigned char *data_one = stbi_load("../shaders/container.jpg", &widths[0], &heights[0],
      &nrChannels[0], 0);

    glGenTextures(1, &textures[0]);
    glBindTexture(GL_TEXTURE_2D, textures[0]);

    if(data_one) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, widths[0], heights[0], 0, GL_RGB,
        GL_UNSIGNED_BYTE, data_one);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture one" << std::endl;
    }

    stbi_image_free(data_one);
  }
}

void generate_buffers(unsigned int *VAO, unsigned int *VBO)
{
  // unsigned int EBO;
  glGenBuffers(1, VBO);
  glGenVertexArrays(1, VAO);
  // glGenBuffers(1, &EBO);

  glBindVertexArray(*VAO);

  glBindBuffer(GL_ARRAY_BUFFER, *VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    (void*)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
    (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
}

void generate_pbo(const cv::Mat& frame) {
  const int width = frame.cols;
  const int height = frame.rows;

  glGenTextures(1, &texture_id);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)nullptr);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Init PBO (GlBufferData): ";
    print_error(error);
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

  const int size_bytes =
    frame.total() * (frame.elemSize() + 1);
  glGenBuffers(1, &PBO);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
  glBufferData(GL_PIXEL_UNPACK_BUFFER, size_bytes, nullptr, GL_STREAM_DRAW);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Init PBO (GlBufferData): ";
    print_error(error);
  }

  glBindTexture(GL_TEXTURE_2D, 0);
}

void update_buf(void* dst, const cv::Mat& frame) {
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

void reload_pbo(const cv::Mat& frame) {

  const int width = frame.cols;
  const int height = frame.rows;
  const int size_bytes =
    frame.total() * (frame.elemSize() + 1);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glBindTexture(GL_TEXTURE_2D, texture_id);
  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);

  // glActiveTexture(GL_TEXTURE0);
  glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, (void*)nullptr);
  error = glGetError();
  if (error != GL_NO_ERROR) {
    std::cout << "Update PBO (GlTexSubImage): ";
    print_error(error);
  }

  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, PBO);
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
    update_buf(ptr, frame);
    // glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

    if(!glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER)) {
      std::cout << "glUnmapBuffer failed" << std::endl;
    }
  }


  glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
  glBindTexture(GL_TEXTURE_2D, 0);

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

  if (g_isFirst) // initially set to true
  {
    g_lastX = xpos;
    g_lastY = ypos;
    g_isFirst = false;
  }

  float xoffset = xpos - g_lastX;
  float yoffset = g_lastY - ypos;

  g_lastX = xpos;
  g_lastY = ypos;

  const float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  g_yaw += xoffset;
  g_pitch += yoffset;

  if(g_pitch > 89.0f)
    g_pitch = 89.0f;
  if(g_pitch < -89.0f)
    g_pitch = -89.0f;

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  g_zoom -= (float)yoffset;
  if (g_zoom < 1.0f)
    g_zoom = 1.0f;
  if (g_zoom > 45.0f)
    g_zoom = 45.0f;
}

int main()
{
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  InitReturn win_obj = init("Testing PBO");

  if(win_obj.status == -1) {
    return -1;
  }

  GLFWwindow* window = win_obj.window;

  Shader shader("../shaders/simple.vert", "../shaders/simple.frag");

  // int widths[1], heights[1], nrChannels[1];
  // unsigned int textures[1];
  // load_textures(widths, heights, nrChannels, textures);

  cv::VideoCapture video;
  video.open(0);
  video.set(cv::CAP_PROP_FRAME_WIDTH, camera_width);
  video.set(cv::CAP_PROP_FRAME_HEIGHT, camera_height);
  cv::Mat frame;

  if(!video.isOpened()) {
    throw std::runtime_error("Cannot open camera");
  }

  video >> frame;
  if(frame.empty()) {
    throw std::runtime_error("Frame empty");
  }

  // PBO stuff
  glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
  generate_pbo(frame);

  unsigned int VAO, VBO;
  generate_buffers(&VAO, &VBO);

  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  projection = glm::perspective(glm::radians(g_zoom), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int model_id = glGetUniformLocation(shader.get_program(), "model");
  unsigned int view_id = glGetUniformLocation(shader.get_program(), "view");
  unsigned int projection_id = glGetUniformLocation(shader.get_program(), "projection");

  shader.use();

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

  glUniform1i(glGetUniformLocation(shader.get_program(), "texture_one"), 0);
  shader.setInt("texture_one", 0);

  float cameraSpeed = 0.05f;
  float deltaTime = 0.0f; // Time between current frame and last frame
  float lastFrame = 0.0f; // Time of last frame

  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  while(!glfwWindowShouldClose(window)) {
    processInput(window);
    auto input = processInputWASD(window);

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    cameraSpeed = 2.5f * deltaTime;

    // Enable depth test for z buffer
    // Render z values first
    glEnable(GL_DEPTH_TEST);

    // Render commands here
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // Clear Buffer so previous frame is not stored
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    // Explicitly use first texture.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    glBindVertexArray(VAO);

    glm::vec3 direction;
    direction.x = cos(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    direction.y = sin(glm::radians(g_pitch));
    direction.z = sin(glm::radians(g_yaw)) * cos(glm::radians(g_pitch));
    cameraFront = glm::normalize(direction);

    if(input.w) {
      cameraPos += cameraSpeed * cameraFront;
    } else if(input.s) {
      cameraPos -= cameraSpeed * cameraFront;
    } else if(input.a) {
      cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    } else if(input.d) {
      cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) *
        cameraSpeed;
    }

    video >> frame;
    reload_pbo(frame);

    {
      model = glm::mat4(1.0f);
      view = glm::mat4(1.0f);

      model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
      projection = glm::perspective(glm::radians(g_zoom), 800.0f / 600.0f, 0.1f, 100.0f);

      glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

      glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteProgram(shader.get_program());

  glfwTerminate();

  return 0;

}
