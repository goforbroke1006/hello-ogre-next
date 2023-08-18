#!/bin/bash

OGRE_VERSION=v2.3.1 #
CXX_STANDARD=14     # 11 | 14
BUILD_TYPE=Release  # Debug | Release

set -e

#sudo apt-get update

sudo apt-get install -y \
  libfreetype6-dev libfreeimage-dev libzzip-dev libxrandr-dev libxaw7-dev freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-xcb-dev \
  libxcb-keysyms1-dev doxygen graphviz python3-clang libsdl2-dev cmake ninja-build git

sudo apt-get install -y libxcb-randr0-dev

mkdir -p ~/OGRECave

if [[ ! -d ~/OGRECave/ogre-next-deps ]]; then
  git clone --recurse-submodules --shallow-submodules https://github.com/OGRECave/ogre-next-deps ~/OGRECave/ogre-next-deps
fi

(
  cd ~/OGRECave/ogre-next-deps
  mkdir -p build
  cd build
  cmake ../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  # Optionally configure build to use distro packages freeimage, freetype, and zlib if installed
  # cmake -D OGREDEPS_BUILD_FREEIMAGE=0 -D OGREDEPS_BUILD_FREETYPE=0 -D OGREDEPS_BUILD_ZLIB=0 ../
  make
  sudo make install
)

if [[ ! -d ~/OGRECave/ogre-next ]]; then
  git clone -b "${OGRE_VERSION}" --single-branch https://github.com/OGRECave/ogre-next.git ~/OGRECave/ogre-next
fi

(
  cd ~/OGRECave/ogre-next
  ln -s ../ogre-next-deps/build/ogredeps Dependencies || true
  mkdir -p build
  cd build

  # Build Debug
  #mkdir -p Debug
  #cd Debug
  #cmake -D OGRE_BUILD_SAMPLES2=1 -D OGRE_CONFIG_THREAD_PROVIDER=0 -D OGRE_CONFIG_THREADS=0 -D CMAKE_BUILD_TYPE=Debug ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  #make
  #sudo make install

  mkdir -p ./${BUILD_TYPE}
  cd ./${BUILD_TYPE}
  #cmake -D OGRE_BUILD_SAMPLES2=1 -D OGRE_CONFIG_THREAD_PROVIDER=0 -D OGRE_CONFIG_THREADS=0 -D CMAKE_BUILD_TYPE=${BUILD_TYPE} ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  cmake -D CMAKE_BUILD_TYPE=${BUILD_TYPE} ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  make
  sudo make install
)
