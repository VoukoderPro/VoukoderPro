cmake_minimum_required(VERSION 3.27)

if(onGitHubActions)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_INSTALL_OPTIONS "--x-buildtrees-root=D:/v/")
endif ()

project(x264_encoder_plugin_cmake LANGUAGES CXX)

include(FetchContent)

set(PLUGIN_NAME "voukoder_pro")

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED COMPONENTS filesystem)
#find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network UiTools)



#pkg_check_modules(X264 REQUIRED x264)

set(SRCS
        Assets.cpp
        EncoderNode.cpp
        FilterNode.cpp
        InputNode.cpp
        JavaScript.cpp
        MuxerNode.cpp
        OutputNode.cpp
        Performance.cpp
        PerformanceManager.cpp
        PostProcNode.cpp
        Router.cpp
        SceneManager.cpp
        Telemetry.cpp
        voukoderpro.cpp)


add_library(helloworld
        SHARED
        ${SRCS}
)


target_include_directories(helloworld PUBLIC
        ${PROJECT_SOURCE_DIR}/../3rdparty/infoware/include
        ${Boost_INCLUDE_DIRS})

target_link_directories(helloworld PUBLIC
        ${PROJECT_SOURCE_DIR}/../3rdparty/infoware/lib)

target_link_libraries(helloworld PUBLIC infoware avformat)
#
#target_link_libraries(Foo PRIVATE Qt6::Core Qt6::Gui Qt6::Network Qt6::Widgets Qt6::UiTools)
#
#target_link_libraries(helloworld PRIVATE Qt6::Core Qt6::Gui Qt6::Network Qt6::Widgets Qt6::UiTools Foo ${Boost_LIBRARIES})

#if ((CMAKE_CXX_COMPILER_ID STREQUAL "Clang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
#    add_compile_options(-fvisibility=hidden)
#endif ()

#include_directories(${Boost_INCLUDE_DIRS})
#
#target_link_libraries(IOPlugin PRIVATE ${Boost_LIBRARIES})
#
#if((${CMAKE_SYSTEM_NAME} STREQUAL "Windows" OR ${CMAKE_SYSTEM_NAME} EQUAL "MSYS") AND ${CMAKE_SIZEOF_VOID_P} EQUAL 8)
#    set(DR_ARCH "Win64")
#elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
#    set(DR_ARCH "MacOS")
#elseif(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
#    set(DR_ARCH "Linux-x86-64")
#else()
#    message(FATAL_ERROR "Unsupported target platform.")
#endif()
#
#set_target_properties(IOPlugin
#        PROPERTIES
#        PREFIX ""
#        OUTPUT_NAME ${PLUGIN_NAME}
#        SUFFIX ".dvcp"
#        LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${PLUGIN_NAME}.dvcp.bundle/Contents/${DR_ARCH}"
#)