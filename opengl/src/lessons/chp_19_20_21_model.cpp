#include "lessons/chp_19_20_21_model.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "image/stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

#include <iostream>
#include <vector>

namespace {

const float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
      0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
      0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
      0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
      0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
      0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float diffuseVal = 0.5f;

// glm::vec3 light_pos(1.0f, 1.0f, 0.0f);

std::vector<glm::vec3> lights = {
  {1.0f, 1.0f, 0.0f},
  {-1.0f, -1.0f, 1.0f}
};

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  // make sure the viewport matches the new window dimensions; note that width and 
  // height will be significantly larger than specified on retina displays.
  glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
  camera.ProcessMouseMovement(xposIn, yposIn);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void print_vec3(const glm::vec3& pos) {
  std::cout << 
    "[" << 
    pos.x << " " <<
    pos.y << " " <<
    pos.z << " " <<
    "]" <<
  std::endl;
}

void moveLight(GLFWwindow *window)
{
  double step = 0.05;

  // Z direction
  if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
    lights[0] += glm::vec3(0.0, 0.0, -step);
  }

  if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
    lights[0] += glm::vec3(0.0, 0.0, step);
  }

  // y direction
  if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
    lights[0] += glm::vec3(0.0, step, 0.0);
  }

  if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
    lights[0] += glm::vec3(0.0, -step, 0.0);
  }

  // x direction
  if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
    lights[0] += glm::vec3(-step, 0.0, 0.0);
  }

  if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
    lights[0]+= glm::vec3(step, 0.0, 0.0);
  }

  // if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
  //   diffuseVal -= 0.01;
  //   std::cout << diffuseVal << std::endl;
  // }

  // if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
  //   diffuseVal += 0.01;
  //   std::cout << diffuseVal << std::endl;
  // }
}

std::string get_uniform_array_name(
  const int iteration,
  const std::string& property
)
{
  return "point_lights[" + std::to_string(iteration) + "]" + "." + property;
}

GLFWwindow* init() {
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
  GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Renderer", NULL, NULL);
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

  return window;
}

} // namespace

int chp_19_20_21_model()
{
  GLFWwindow* window = init();

  if(!window) {
    return 1;
  }

  stbi_set_flip_vertically_on_load(true);

  Shader shader(
    "../shaders/chapter_19_20_21/model.vert",
    "../shaders/chapter_19_20_21/model.frag"
  );

  Shader light_cube_shader(
    "../shaders/chapter_19_20_21/lightCube.vert",
    "../shaders/chapter_19_20_21/lightCube.frag"
  );

  // Setup light cube
  unsigned int light_cube_vao;
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glGenVertexArrays(1, &light_cube_vao);
  glBindVertexArray(light_cube_vao);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);



  // Need to do path
  Model model_backpack("../models/backpack/backpack.obj");

  // render loop
  while (!glfwWindowShouldClose(window))
  {
    float currentFrame = static_cast<float>(glfwGetTime());
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    // input
    // -----
    processInput(window);
    moveLight(window);

    shader.use();

    // Set light properties
    shader.setVec3("viewPos", camera.Position);

    shader.setVec3("directional_light.direction", -0.2f, -1.0f, -0.3f);

    shader.setVec3("directional_light.ambient", 0.3f, 0.3f, 0.3f); // Original: 0.3
    shader.setVec3("directional_light.diffuse", 0.5f, 0.5f, 0.5f);
    shader.setVec3("directional_light.specular", 0.5f, 0.5f, 0.5f);

    shader.setInt("point_lights_size", lights.size());
    for(unsigned int i = 0; i < lights.size(); ++i) {
      shader.setVec3(get_uniform_array_name(i, "position"), lights[i]);
      
      shader.setVec3(get_uniform_array_name(i, "ambient"), 0.4f, 0.4f, 0.1);
      shader.setVec3(get_uniform_array_name(i, "diffuse"), 0.5f, 0.5f, 0.5f);
      shader.setVec3(get_uniform_array_name(i, "specular"), 1.0f, 1.0f, 1.0f);

      shader.setFloat(get_uniform_array_name(i,"constant"), 1.0f);
      shader.setFloat(get_uniform_array_name(i, "linear"), 0.7f);
      shader.setFloat(get_uniform_array_name(i, "quadratic"), 1.8f);
    }

    // render
    // ------
    // glClearColor(0.9, 0.85f, 0.19f, 1.0f);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(0.8f));
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setMat4("model", model);

    model_backpack.Draw(shader);

    // Another instance
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-3.0f, -3.0f, -0.0f));
    model = glm::scale(model, glm::vec3(0.8f));

    shader.setMat4("model", model);
    model_backpack.Draw(shader);

    // Render light cube
    light_cube_shader.use();

    for(unsigned int i = 0; i < lights.size(); ++i) {
      model = glm::mat4(1.0f);
      model = glm::translate(model, lights[i]);
      model = glm::scale(model, glm::vec3(0.2f));

      light_cube_shader.setMat4("model", model);
      light_cube_shader.setMat4("projection", projection);
      light_cube_shader.setMat4("view", view);

      glBindVertexArray(light_cube_vao);
      glDrawArrays(GL_TRIANGLES, 0, 36);  
    }

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();
  return 0;
}