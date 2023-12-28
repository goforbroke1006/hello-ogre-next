#!/bin/bash

OGRE_NEXT_BRANCH=v2.3.3
CXX_STANDARD=14     # 11 | 14

set -e

#
# Install Clang 17
#
if [[ ! -f /usr/bin/clang++-17 ]]; then
  wget https://apt.llvm.org/llvm.sh
  chmod u+x llvm.sh
  sudo ./llvm.sh 17
  clang-17 --version
fi

echo "CMAKE_C_COMPILER   = /usr/bin/clang-17"
echo "CMAKE_CXX_COMPILER = /usr/bin/clang++-17"

sudo apt-get update

sudo apt-get install -y \
  git ninja-build build-essential

sudo apt remove cmake -y || true
sudo snap install cmake --classic || sudo snap refresh cmake --classic

sudo apt-get install -y \
  libfreetype6-dev libfreeimage-dev libzzip-dev libxrandr-dev libxaw7-dev freeglut3-dev libgl1-mesa-dev libglu1-mesa-dev libx11-xcb-dev \
  libxcb-keysyms1-dev doxygen graphviz python3-clang libsdl2-dev ninja-build git \
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
  cmake -G Ninja \
    -DCMAKE_MAKE_PROGRAM=ninja \
    -DCMAKE_C_COMPILER=/usr/bin/clang-17 \
    -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 \
    -DCMAKE_CXX_STANDARD=${CXX_STANDARD} \
    ../
  cmake --build .
#  cmake -D OGREDEPS_BUILD_FREEIMAGE=0 -D OGREDEPS_BUILD_FREETYPE=0 -D OGREDEPS_BUILD_ZLIB=0 ../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
  # Optionally configure build to use distro packages freeimage, freetype, and zlib if installed
  # cmake -D OGREDEPS_BUILD_FREEIMAGE=0 -D OGREDEPS_BUILD_FREETYPE=0 -D OGREDEPS_BUILD_ZLIB=0 ../
#  make
)

if [[ ! -d ./third_party/ogre-next ]]; then
  git clone --depth 1 --single-branch --branch "${OGRE_NEXT_BRANCH}" https://github.com/OGRECave/ogre-next \
    ./third_party/ogre-next
else
  (cd ./third_party/ogre-next && git pull)
fi

(
  cd ./third_party/ogre-next
  rm Dependencies
  ln -s ../ogre-next-deps/build/ogredeps Dependencies
  mkdir -p build
  cd build

  (
    # Build Debug
    mkdir -p Debug
    cd Debug
    cmake -G Ninja \
      -DCMAKE_MAKE_PROGRAM=ninja \
      -DCMAKE_C_COMPILER=/usr/bin/clang-17 \
      -DCMAKE_CXX_COMPILER=/usr/bin/clang++-17 \
      -DOGRE_BUILD_SAMPLES2=1 \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_CXX_STANDARD=${CXX_STANDARD} \
      ../../
#    cmake -D OGRE_BUILD_SAMPLES2=1 -D OGRE_CONFIG_THREAD_PROVIDER=0 -D OGRE_CONFIG_THREADS=0 -D CMAKE_BUILD_TYPE=Debug ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
    cmake --build .
    sudo cmake --install . --config Debug
  )

#  (
#    # Build Release
#    mkdir -p Release
#    cd Release
#    #cmake -D OGRE_BUILD_SAMPLES2=1 -D OGRE_CONFIG_THREAD_PROVIDER=0 -D OGRE_CONFIG_THREADS=0 -D CMAKE_BUILD_TYPE=Release ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
#    cmake -DCMAKE_BUILD_TYPE=Release ../../ -DCMAKE_CXX_STANDARD=${CXX_STANDARD}
#    make
#    sudo make install
#  )
)
