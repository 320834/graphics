# ray-tracing

A test repo for learning ray tracing

## Setup

Compiler: clang v14.0.0

CPP Standard: c++20

If on linux, make sure that g++-12 is installed. Clang 14 default seems to use headers from g++12.

To install g++-12, if on debian use `sudo apt install g++-12`

MacOS should work out of the box.

Windows I don't know.

**Steps**
1. clone repo
2. mkdir build && cd build
3. cmake ..
4. make

This link helped me: https://stackoverflow.com/questions/26333823/clang-doesnt-see-basic-headers