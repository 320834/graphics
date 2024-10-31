dir_release="release"
dir_debug="debug"

build() {
  # Build
  if [ ! -d "$dir_release" ]; then
    mkdir "$dir_release" && cd "$dir_release"
    cmake -D CMAKE_BUILD_TYPE=Release ..
    cd ..
  fi

  # Debug
  if [ ! -d "$dir_debug" ]; then
    mkdir "$dir_debug" && cd "$dir_debug"
    cmake -D CMAKE_BUILD_TYPE=Debug ..
    cd ..
  fi
}

shaders() {
  if [ -d "$dir_release" ]; then
    cp -rf shaders/ "$dir_release"
  fi

  if [ -d "$dir_debug" ]; then
    cp -rf shaders/ "$dir_debug"
  fi
}

glm() {
  if [ -d include ]; then
    pushd include

    git clone https://github.com/g-truc/glm.git

    popd

  fi
}

if [ "$1" == "build" ]; then
  echo "Starting building setup"
  build
elif [ "$1" == "glm" ]; then
  echo "Downloading glm"
  glm
else
  echo "Unknown command: $1 | Or no arguments entered"
fi
