cmake_minimum_required(VERSION 3.22)
project(cvp)

# Add dependencies here

find_package(engine CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++)

set(SOURCES
  main.cpp
  src/main_scene.cpp
)

set(HEADERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${HEADERS_DIR})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} PUBLIC engine::engine)