cmake_minimum_required(VERSION 3.22)
project(snake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++)

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
  src/main.cpp
  src/engine/engine.cpp
  src/engine/char_3d.cpp
  src/snake_scene.cpp
  src/main_menu_scene.cpp
  src/game_end_scene.cpp
)

add_executable(snake ${GRAPHICS_SOURCES} ${DEPENDENCY_SOURCES})
target_link_libraries(snake PRIVATE glfw)

target_include_directories(snake PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/glfw-src/include)
