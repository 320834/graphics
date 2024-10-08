cmake_minimum_required(VERSION 3.22)
project(graphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++)
# set(CMAKE_EXE_LINKER_FLAGS "-lGL")

# All module for fetch content
include(FetchContent)

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

FetchContent_MakeAvailable(glfw)

# Set header dir
set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${HEADERS_DIR})

# Set glm headers
set(HEADERS_GLM_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/glm)
include_directories(${HEADERS_GLM_DIR})


set(DEPENDENCY_SOURCES
  src/glad/glad.c
  src/image/stb_image.cpp
)

set(GRAPHICS_SOURCES
  main.cpp
  src/lessons/chp_5_hello_triangle.cpp
  src/lessons/chp_5_exercises.cpp
  src/lessons/chp_6_shaders.cpp
  src/lessons/chp_6_exercises.cpp
  src/lessons/chp_7_textures.cpp
  src/lessons/chp_7_exercises.cpp
  src/lessons/chp_8_transform.cpp
  src/lessons/chp_8_exercises.cpp
  src/lessons/chp_9_coordinate_systems.cpp
  src/lessons/chp_9_exercises.cpp
  src/lessons/chp_10_camera.cpp
  src/lessons/chp_10_exercises.cpp
  src/experiment/index.cpp
  src/experiment/engine.cpp
  src/experiment/snake_scene.cpp
  src/experiment/char_3d.cpp
  src/experiment/main_menu_scene.cpp
  src/experiment/game_end_scene.cpp
)

add_executable(graphics ${GRAPHICS_SOURCES} ${DEPENDENCY_SOURCES})
target_link_libraries(graphics PRIVATE glfw)

target_include_directories(graphics PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/glfw-src/include)
