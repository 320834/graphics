cmake_minimum_required(VERSION 3.22)
project(engine)

# ================================
# Dependencies
include(FetchContent)

FetchContent_Declare(
  glfw
  URL https://github.com/glfw/glfw/archive/refs/tags/3.4.zip # v3.4
)

FetchContent_Declare(
  glm
  URL https://github.com/g-truc/glm/archive/refs/tags/1.0.1.zip # v1.0.1
)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip # v1.15.2
)

FetchContent_Declare(
  stb_image
  DOWNLOAD_NO_EXTRACT TRUE
  URL https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image.h
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(stb_image)

# ================================
# Build library

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++)

# Add automatically to source code
set(DEPENDENCY_SOURCES
  src/glad/glad.c
  src/image/stb_image.cpp
)

set(ENGINE_SOURCES
  src/engine.cpp
)

add_library(${PROJECT_NAME} SHARED)

target_sources(
  ${PROJECT_NAME}
  PRIVATE
    ${ENGINE_SOURCES}
    ${DEPENDENCY_SOURCES}
)

target_include_directories(
  ${PROJECT_NAME}
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/src
      ${stb_image_SOURCE_DIR} # Add header to include path
    PUBLIC
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

# ================================
# Build executable

set(ENGINE_EXEC_NAME
  ${PROJECT_NAME}_main
)

add_executable(${ENGINE_EXEC_NAME} main.cpp)
target_link_libraries(${ENGINE_EXEC_NAME} PRIVATE ${PROJECT_NAME})
target_link_libraries(${ENGINE_EXEC_NAME} PRIVATE glm)

# ================================
# Test setup

set(ENGINE_TEST_NAME
  ${PROJECT_NAME}_td 
)

enable_testing()

set(ENGINE_TEST_SOURCES
  tests/test_main.cpp
)

add_executable(
  ${ENGINE_TEST_NAME}
  ${ENGINE_TEST_SOURCES}
)

target_link_libraries(${ENGINE_TEST_NAME} PRIVATE GTest::gtest_main)
# Link engine library
target_link_libraries(${ENGINE_TEST_NAME} PRIVATE ${PROJECT_NAME})

include(GoogleTest)
gtest_discover_tests(${ENGINE_TEST_NAME})


