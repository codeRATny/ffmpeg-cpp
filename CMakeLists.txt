project(ffmpeg-cpp)
cmake_minimum_required(VERSION 3.1.3)

file(GLOB SRC_LIST
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
  ${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

set(CMAKE_CXX_STANDARD 20)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

add_library(ffmpeg-cpp STATIC ${SRC_LIST})

include(FindPkgConfig)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBAV REQUIRED IMPORTED_TARGET
    libavformat
    libavcodec
    libavutil
    libswscale
    libswresample
)

target_link_libraries(ffmpeg-cpp PkgConfig::LIBAV)
target_include_directories(ffmpeg-cpp
  PRIVATE
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>")

add_subdirectory(${PROJECT_SOURCE_DIR}/examples)
