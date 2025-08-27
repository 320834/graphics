cmake_minimum_required(VERSION 3.22)
project(cvp)

# Add dependencies here

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_COMPILER clang++)

set(SOURCES
  main.cpp
)

add_executable(cvp ${SOURCES})

find_package(engine CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC engine)