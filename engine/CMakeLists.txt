cmake_minimum_required(VERSION 3.22)
project(engine VERSION 0.1.0 LANGUAGES C CXX)

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

FetchContent_Declare(
  assimp
  GIT_REPOSITORY https://github.com/assimp/assimp.git
  GIT_TAG v6.0.2
)

FetchContent_MakeAvailable(glfw)
FetchContent_MakeAvailable(glm)
FetchContent_MakeAvailable(googletest)
FetchContent_MakeAvailable(stb_image)
FetchContent_MakeAvailable(assimp)

# Links for shared object. Linux mint has weirdness
# where it has issues linking this shared object with static libz
find_package(ZLIB REQUIRED)

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
  src/shapes/cube.cpp
  src/shapes/dynamic.cpp
  src/shapes/assimp/model.cpp
  src/shapes/assimp/mesh.cpp
  src/engine.cpp
  src/scene_manager.cpp
  src/timer.cpp
  src/texture_manager.cpp
  src/lights/directional.cpp
  src/lights/point.cpp
  src/lights/spot.cpp
  src/scene.cpp
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

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glm)
target_link_libraries(${PROJECT_NAME} PUBLIC assimp)
# For linking shared object
target_link_libraries(${PROJECT_NAME} PRIVATE ZLIB::ZLIB)

# ================================
# Build demo executable

set(DEMO_EXEC_NAME
  demo
)

set(DEMO_EXEC_SOURCES
  demo_game/main.cpp
  demo_game/src/generic_scene.cpp
)

set(DEMO_HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/demo_game/include)
include_directories(${DEMO_HEADERS_DIR})

add_executable(${DEMO_EXEC_NAME} ${DEMO_EXEC_SOURCES})
target_link_libraries(${DEMO_EXEC_NAME} PRIVATE ${PROJECT_NAME}) # Link engine

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


# Installation

include(CMakePackageConfigHelpers)

install(TARGETS ${PROJECT_NAME}
  EXPORT engineTargets
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT engineTargets
  FILE engineTargets.cmake
  NAMESPACE engine::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/engine
)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/engineConfigVersion.cmake"
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion
)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/engineConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/engineConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/engine
)

install(FILES
  "${CMAKE_CURRENT_BINARY_DIR}/engineConfig.cmake"
  "${CMAKE_CURRENT_BINARY_DIR}/engineConfigVersion.cmake"
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/engine
)