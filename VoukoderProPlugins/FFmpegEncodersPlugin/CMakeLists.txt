cmake_minimum_required(VERSION 3.27)
project(x264_encoder_plugin_cmake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)

find_package(Boost REQUIRED)

set(SRCS
        plugin.cpp)

if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    add_compile_options(-fvisibility=hidden)
endif()

add_library(FfmpegEncodersPlugin
        SHARED
        ${SRCS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(FfmpegEncodersPlugin PRIVATE ${Boost_LIBRARIES})