#!/bin/bash

OGRE_NEXT_VERSION=v2.3.3
CXX_STANDARD=14     # 11 | 14

set -e

sudo apt-get update

sudo apt-get install -y \
  git ninja-build build-essential

sudo apt-get install -y \
  libfreetype6-dev libfreeimage-dev libzzip-dev libxrandr-dev libxaw7-dev freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-xcb-dev \
  libxcb-keysyms1-dev doxygen graphviz python3-clang libsdl2-dev cmake ninja-build git \
  rapidjson-dev

#
#
#

mkdir -p ./third_party/

if [[ ! -d ./third_party/ogre-next-deps ]]; then
  git clone --depth 1 --single-branch --recurse-submodules --shallow-submodules https://github.com/OGRECave/ogre-next-deps \
    ./third_party/ogre-next-deps
else
  (cd ./third_party/ogre-next-deps && git pull)
fi

(
  cd ./third_party/ogre-next-deps
  mkdir -p build
  cd build
#  cmake ../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  cmake -D OGREDEPS_BUILD_FREEIMAGE=0 -D OGREDEPS_BUILD_FREETYPE=0 -D OGREDEPS_BUILD_ZLIB=0 ../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  # Optionally configure build to use distro packages freeimage, freetype, and zlib if installed
  # cmake -D OGREDEPS_BUILD_FREEIMAGE=0 -D OGREDEPS_BUILD_FREETYPE=0 -D OGREDEPS_BUILD_ZLIB=0 ../
  make
)

if [[ ! -d ./third_party/ogre-next ]]; then
  git clone --depth 1 --single-branch --branch "${OGRE_NEXT_VERSION}" https://github.com/OGRECave/ogre-next \
    ./third_party/ogre-next
else
  (cd ./third_party/ogre-next && git pull)
fi

(
  cd ./third_party/ogre-next
  ln -s ../ogre-next-deps/build/ogredeps Dependencies || true
  mkdir -p build
  cd build

  (
    # Build Debug
    mkdir -p Debug
    cd Debug
    cmake -D OGRE_BUILD_SAMPLES2=1 -D OGRE_CONFIG_THREAD_PROVIDER=0 -D OGRE_CONFIG_THREADS=0 -D CMAKE_BUILD_TYPE=Debug ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
    make
    sudo make install
  )

  (
    # Build Release
    mkdir -p Release
    cd Release
    #cmake -D OGRE_BUILD_SAMPLES2=1 -D OGRE_CONFIG_THREAD_PROVIDER=0 -D OGRE_CONFIG_THREADS=0 -D CMAKE_BUILD_TYPE=Release ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
    cmake -DCMAKE_BUILD_TYPE=Release ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
    make
    sudo make install
  )
)
