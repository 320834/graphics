cmake_minimum_required(VERSION 3.22)
project(graphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_EXE_LINKER_FLAGS "-lGL")

# All module for fetch content
include(FetchContent)

FetchContent_Declare(
  GLFW
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 3.4
)

FetchContent_MakeAvailable(GLFW)

find_package(OpenGL REQUIRED)

# Set header dir
set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${HEADERS_DIR})

set(GRAPHICS_SOURCES
  main.cpp
  src/glad/glad.c
)
# message(${OPENGL_INCLUDE_DIRS})
# target_include_directories (graphics PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
add_executable(graphics ${GRAPHICS_SOURCES})
target_link_libraries(graphics PUBLIC ${GLFW_LIBS})
# target_link_libraries(graphics PUBLIC ${OPENGL_LIBS})

target_include_directories(graphics PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/_deps/glfw-src/include)
# target_include_directories(graphics PRIVATE ${OPENGL_INCLUDE_DIRS})