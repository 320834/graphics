dir_release="release"
dir_debug="debug"

# Build
if [ ! -d "$dir_release" ]; then
  mkdir "$dir_release" && cd "$dir_release"
  cmake -D CMAKE_BUILD_TYPE=Release ..
fi

# Debug
if [ ! -d "$dir_debug" ]; then
  mkdir "$dir_debug" && cd "$dir_debug"
  cmake -D CMAKE_BUILD_TYPE=Debug ..
fi
