cmake_minimum_required(VERSION 3.27)

if(onGitHubActions)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../vcpkg/scripts/buildsystems/vcpkg.cmake")
    set(VCPKG_INSTALL_OPTIONS "--x-buildtrees-root=D:/v/")
endif ()

project(voukoder_pro_dll LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

find_package(Boost REQUIRED COMPONENTS filesystem)

# infoware
FetchContent_Declare(
        GetInfoware
        GIT_REPOSITORY https://github.com/ThePhD/infoware.git
        GIT_TAG v0.6.0
)
FetchContent_MakeAvailable(GetInfoware)

set_target_properties(infoware PROPERTIES POSITION_INDEPENDENT_CODE True)


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

add_library(voukoderpro
        SHARED
        ${SRCS}
)

target_include_directories(voukoderpro PUBLIC
        ${Boost_INCLUDE_DIRS})

IF(WIN32)
    target_include_directories(voukoderpro PUBLIC
            ${PROJECT_SOURCE_DIR}/../3rdparty/ffmpeg-6.1-release/include)
    target_link_directories(voukoderpro PUBLIC
            ${PROJECT_SOURCE_DIR}/../3rdparty/ffmpeg-6.1-release/lib)
    target_link_libraries(voukoderpro PUBLIC avformat-voukoderpro infoware ${Boost_LIBRARIES})
ELSE ()
    target_link_libraries(voukoderpro PUBLIC avformat infoware ${Boost_LIBRARIES})
ENDIF ()

set_target_properties(voukoderpro PROPERTIES PREFIX "")