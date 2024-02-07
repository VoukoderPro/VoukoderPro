cmake_minimum_required(VERSION 3.27)
project(x264_encoder_plugin_cmake LANGUAGES CXX)

set(PLUGIN_NAME "voukoder_pro")

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)

find_package(Boost REQUIRED)

#pkg_check_modules(X264 REQUIRED x264)

set(SRCS
        plugin.cpp
        video_encoder.cpp
        voukoder_container.cpp
        ioplugin_sdk/host_api.cpp
        ioplugin_sdk/plugin_api.cpp)

if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
    add_compile_options(-fvisibility=hidden)
endif()

add_library(IOPlugin
        SHARED
        ${SRCS})

include_directories(${Boost_INCLUDE_DIRS})

target_link_libraries(IOPlugin PRIVATE ${Boost_LIBRARIES})

if((${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR ${CMAKE_SYSTEM_NAME} EQUAL "MSYS") AND ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
    set(DR_ARCH "Win64")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    set(DR_ARCH "MacOS")
elseif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(DR_ARCH "Linux-x86-64")
else()
    message(FATAL_ERROR "Unsupported target platform.")
endif()

set_target_properties(IOPlugin
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME ${PLUGIN_NAME}
        SUFFIX ".dvcp"
        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${PLUGIN_NAME}.dvcp.bundle/Contents/${DR_ARCH}"
)