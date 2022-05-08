cmake_minimum_required(VERSION 3.9)
project(HMMRWAR
  VERSION 0.0.1
  LANGUAGES CXX
  DESCRIPTION "HMMRWAR: A HMMRATAC implementation in C++"
)

add_executable(${PROJECT_NAME}
  src/main.cpp src/argparser.cpp
  src/io/genome_reader.cpp
)

include(FetchContent)
FetchContent_Declare(stoch
  GIT_REPOSITORY https://github.com/KorfLab/StochHMM
  GIT_TAG v0.38
)

FetchContent_Populate(stoch)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -pedantic -std=c++17")

# temporary
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
target_include_directories(${PROJECT_NAME} PUBLIC inc/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/_deps/stoch-src/src/)
