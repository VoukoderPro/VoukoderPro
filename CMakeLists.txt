cmake_minimum_required(VERSION 3.28)
project("Voukoder Pro")

SET(CMAKE_INSTALL_PREFIX install CACHE PATH "install locally" FORCE)

add_subdirectory(VoukoderPro)
add_subdirectory(Designer)
add_subdirectory(AppPlugins)

add_custom_target(FullBuild)
add_dependencies(FullBuild voukoderpro VoukoderDesigner AppPlugins)
