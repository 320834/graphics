#include "lessons/chp_22_depth_testing.h"

// #include "utils.h"
#include "callbacks.h"
#include "shader.h"
#include "image/stb_image.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace {

float vertices[] = {
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
  0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
  -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
  -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
};

glm::vec3 cubePositions[] = {
  glm::vec3( 1.0f, 0.0f, -0.5f),
  glm::vec3( -1.0f, -0.0f, -0.7f),
  glm::vec3( 0.0f, -1.0f, -1.0f)
};

float g_lastX = 400;
float g_lastY = 300;
bool g_isFirst = true;
float g_yaw = -90.0f;
float g_pitch = 0.0f;
float g_zoom = 45.0f;

const float SCR_WIDTH = 800;
const float SCR_HEIGHT = 600;

void load_textures(int widths[1], int heights[1], int nrChannels[1], unsigned int textures[1]) {

  {
    unsigned char *data_one = stbi_load("../textures/marble.jpg", &widths[0], &heights[0],
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

GLFWwindow* init(const std::string& name) {
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, name.c_str(), NULL, NULL);
  if (window == NULL)
  {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return nullptr;
  }
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

  // tell GLFW to capture our mouse
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
      std::cout << "Failed to initialize GLAD" << std::endl;
      return nullptr;
  }

  // configure global opengl state
  // -----------------------------
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  return window;
}

} // namespace

int chp_22_depth_testing()
{
  glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  GLFWwindow* window = init("Chapter 22: Depth Testing");

  if(!window) {
    return -1;
  }

  Shader shader("../shaders/chapter_22/vertex.vert", "../shaders/chapter_22/fragment.frag");

  int widths[1], heights[1], nrChannels[1];
  unsigned int textures[1];
  load_textures(widths, heights, nrChannels, textures);

  unsigned int VAO, VBO;
  generate_buffers(&VAO, &VBO);


  glm::mat4 model = glm::mat4(1.0f);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection = glm::mat4(1.0f);

  model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  projection = glm::perspective(glm::radians(g_zoom), 800.0f / 600.0f, 0.1f, 100.0f);

  unsigned int model_id = glGetUniformLocation(shader.get_program(), "model");
  unsigned int view_id = glGetUniformLocation(shader.get_program(), "view");
  unsigned int projection_id = glGetUniformLocation(shader.get_program(), "projection");

  shader.use();

  glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
  glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

  glUniform1i(glGetUniformLocation(shader.get_program(), "ourTextureOne"), 0);
  shader.setInt("ourTextureOne", 0);

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
    glBindTexture(GL_TEXTURE_2D, textures[0]);

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

    for(unsigned int i = 0; i < 3; i++) {
      model = glm::mat4(1.0f);
      view = glm::mat4(1.0f);

      model = glm::translate(model, cubePositions[i]);

      if(i == 2) {
        model = glm::scale(model, glm::vec3(10.0f, 0.2f, 10.0f));
      }

      view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

      projection = glm::perspective(glm::radians(g_zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);

      glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model));
      glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view));
      glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection));

      glDrawArrays(GL_TRIANGLES, 0, 36);
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
