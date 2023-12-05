cmake_minimum_required(VERSION 3.22)
project(RayTracing)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_COMPILER clang++)

include_directories(include)

set(RAYTRACING_SOURCES
  main.cpp
  src/chapter_2.cpp
)

add_executable(tracing ${RAYTRACING_SOURCES})