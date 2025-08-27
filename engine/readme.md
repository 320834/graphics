# Engine (WIP)

Playground for custom render engine

## Conan

conan install . --output-folder=release --build=missing -c tools.system.package_manager:mode=install -c system.package_manager:sudo=True

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

make

Dependencies

glfw - cmake added to dependency.
https://github.com/glfw/glfw

glm - vector header library. Ran bash script to download.
https://github.com/g-truc/glm

stbimage - for loading images
header library. Need to figure out how to download header, because repo is using headers.
Might need to include in source code.

glad.c - Will have to include source code.
