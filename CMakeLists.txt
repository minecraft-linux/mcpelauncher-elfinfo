cmake_minimum_required (VERSION 2.6)

project(mcpelauncher-elfinfo LANGUAGES CXX)

add_library(mcpelauncher-elfinfo-lib include/mcpelauncher/memory_region.h lib/memory_region.cpp include/mcpelauncher/elf_reader.h)
target_include_directories(mcpelauncher-elfinfo-lib PUBLIC include/)

add_executable(mcpelauncher-elfinfo src/main.cpp include/mcpelauncher/minecraft_elf_info.h)
target_link_libraries(mcpelauncher-elfinfo mcpelauncher-elfinfo-lib)

