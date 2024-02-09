cmake_minimum_required(VERSION 3.27)
project(VoukoderDesigner LANGUAGES CXX)

include(FetchContent)

set(CMAKE_CXX_STANDARD 23)

find_package(PkgConfig REQUIRED)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui Network UiTools)

qt_standard_project_setup()

# Qt Property Browser
# TODO: make qttools version as a variable
FetchContent_Declare(
        QtTools
        URL https://download.qt.io/official_releases/qt/6.6/6.6.1/submodules/qttools-everywhere-src-6.6.1.tar.xz
)
FetchContent_GetProperties(QtTools)
if(NOT qttools_POPULATED)
    FetchContent_Populate(QtTools)
endif()

set(PropertyBrowser_SRC
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qtbuttonpropertybrowser.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qteditorfactory.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qtgroupboxpropertybrowser.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowser.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertybrowserutils.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qtpropertymanager.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qttreepropertybrowser.cpp
        ${qttools_SOURCE_DIR}/src/shared/qtpropertybrowser/qtvariantproperty.cpp)

qt_add_library(PropertyBrowser
        STATIC
        ${PropertyBrowser_SRC}
)

target_link_libraries(PropertyBrowser PRIVATE Qt6::Core Qt6::Widgets)

# Node Editor 2
set(PatchNE2Cmake git apply ${CMAKE_CURRENT_SOURCE_DIR}/BuildNodeEditor2.patch)
FetchContent_Declare(
        NodeEditor2
        GIT_REPOSITORY git@github.com:paceholder/nodeeditor.git
        GIT_TAG 2.2.4
        PATCH_COMMAND ${PatchNE2Cmake}
)
FUNCTION( NE2_MakeAvailable )
    set(BUILD_TESTING OFF)
    set(BUILD_EXAMPLES OFF)
    set(BUILD_SHARED_LIBS OFF)
    set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
    FetchContent_MakeAvailable(NodeEditor2)
ENDFUNCTION ()
NE2_MakeAvailable()

# Voukoder Designer
set(SRCS
        aboutdialog.cpp
        components/SceneEditor/nodes/sceneeditorencodernodemodel.cpp
        components/SceneEditor/nodes/sceneeditorfilternodemodel.cpp
        components/SceneEditor/nodes/sceneeditorinputnodemodel.cpp
        components/SceneEditor/nodes/sceneeditormuxernodemodel.cpp
        components/SceneEditor/nodes/sceneeditornodemodel.cpp
        components/SceneEditor/nodes/sceneeditoroutputnodemodel.cpp
        components/SceneEditor/nodes/sceneeditorpostprocnodemodel.cpp
        components/SceneEditor/nodes/ui/encoderpropertiesdialog.cpp
        components/SceneEditor/nodes/ui/filterpropertiesdialog.cpp
        components/SceneEditor/nodes/ui/inputpropertiesdialog.cpp
        components/SceneEditor/nodes/ui/muxerpropertiesdialog.cpp
        components/SceneEditor/nodes/ui/outputpropertiesdialog.cpp
        components/SceneEditor/nodes/ui/postprocpropertiesdialog.cpp
        components/SceneEditor/nodes/ui/propertiesdialog.cpp
        components/SceneEditor/sceneeditorscene.cpp
        components/SceneEditor/sceneeditorview.cpp
        main.cpp
        mainwindow.cpp
        newsdialog.cpp
        performancetestdialog.cpp
        preferences.cpp
        preferencesdialog.cpp
        sceneopendialog.cpp
        scenesavedialog.cpp
        sceneselectdialog.cpp
        testrundialog.cpp)

set(FORMS
        aboutdialog.ui
        components/SceneEditor/nodes/ui/inputpropertiesdialog.ui
        components/SceneEditor/nodes/ui/propertiesdialog.ui
        mainwindow.ui
        newsdialog.ui
        performancetestdialog.ui
        preferencesdialog.ui
        sceneopendialog.ui
        scenesavedialog.ui
        sceneselectdialog.ui
        testrundialog.ui)

qt_add_executable(VoukoderDesigner
        ${SRCS}
        ${FORMS}
        designer.qrc
        fugue.qrc
)

target_compile_definitions(VoukoderDesigner PRIVATE -DNODE_EDITOR_STATIC)

target_include_directories(VoukoderDesigner PRIVATE
        ${PROJECT_SOURCE_DIR}
        ${qttools_SOURCE_DIR}/src/shared
        ${Boost_INCLUDE_DIRS})

target_link_libraries(VoukoderDesigner PRIVATE Qt6::Core Qt6::Gui Qt6::Network Qt6::Widgets Qt6::UiTools PropertyBrowser ${Boost_LIBRARIES} nodes)