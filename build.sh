#!/bin/bash

dir=$PWD
echo "Always coming back to $dir"

if [ "$1" = "-clean" ]; then
  echo 'rm -rf build/* compile_commands.json'
  rm -rf build/* compile_commands.json
fi

if [ ! -d "seqan3" ]; then
  echo 'git clone --recurse-submodules https://github.com/seqan/seqan3.git'
  git clone --recurse-submodules https://github.com/seqan/seqan3.git
fi

if [ ! -d "build" ]; then
  echo 'mkdir build'
  mkdir build
fi

prf=hmmrwar
echo "conan profile show $prf >/dev/null || ( \
  conan profile new $prf --detect && \
  conan profile update settings.compiler.libcxx=libstdc++11 $prf \
)"
conan profile show $prf >/dev/null || ( \
  conan profile new $prf --detect && \
  conan profile update settings.compiler.libcxx=libstdc++11 $prf \
)

if [ ! -f "build/conanbuildinfo.cmake" ]; then
  echo 'cd build && conan install .. --profile hmmrwar'
  echo "cd $dir"
  cd build && conan install .. --profile $prf
  cd $dir
fi

if [ ! -f "compile_commands.json" ]; then
  echo 'cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..'
  cd build && cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
  echo 'sed -i "s#/usr/bin/c++#/usr/bin/g++#" compile_commands.json'
  sed -i "s#/usr/bin/c++#/usr/bin/g++#" compile_commands.json
  echo "cd $dir"
  cd $dir
  echo 'ln -s build/compile_commands.json'
  ln -s build/compile_commands.json
fi

echo 'cd build && cmake --build .'
cd build && cmake --build .
echo "cd $dir"
cd $dir
