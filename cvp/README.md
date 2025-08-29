# CVP

Just to test out engine and opencv

# Install

conan install . --output-folder=release --build=missing -c tools.system.package_manager:mode=install -c tools.system.package_manager:sudo=True

cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

make