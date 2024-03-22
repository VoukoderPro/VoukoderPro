cmake_minimum_required(VERSION 3.28)
project("Voukoder App Plugins")

add_subdirectory(DaVinciResolvePlugin)

add_custom_target(AppPlugins)

add_dependencies(AppPlugins IOPlugin)
