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

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

add_executable(abyss
    src/main.cpp
)

target_link_libraries(abyss PRIVATE PkgConfig::SDL2)
