cmake_minimum_required(VERSION 3.20)

project(ABYSS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Default to Debug so CMAKE_BUILD_TYPE is never blank.
# Override on the command line: cmake -DCMAKE_BUILD_TYPE=Release ...
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

# Generate compile_commands.json for VS Code IntelliSense.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Raylib provides its own CMake package config.
# Installed via: pacman -S mingw-w64-ucrt-x86_64-raylib (version 6.2-1)
# The imported target name is 'raylib' (confirmed from raylib-targets.cmake).
find_package(raylib REQUIRED)

add_executable(abyss
    src/main.cpp
    src/core/Application.cpp
    src/graphics/Framebuffer.cpp
    src/graphics/Renderer.cpp
    src/gameplay/Player.cpp
)

target_include_directories(abyss PRIVATE src)

target_link_libraries(abyss PRIVATE raylib)
