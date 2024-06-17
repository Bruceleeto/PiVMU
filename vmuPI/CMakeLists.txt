cmake_minimum_required(VERSION 3.10)
project(vmuPI VERSION 1.0 DESCRIPTION "vmuPI" LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Use pkg-config to ensure proper detection and configuration
find_package(PkgConfig REQUIRED)

# Find SDL2 and its extensions
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2TTF REQUIRED SDL2_ttf)
pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)

# Include the directories found by pkg-config for SDL2 and its extensions
include_directories(${SDL2_INCLUDE_DIRS} ${SDL2TTF_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} )

add_executable(vmuPI source/main.c source/start_menu.c source/select.c)

# Link the libraries found by pkg-config for SDL2 and its extensions
target_link_libraries(vmuPI PRIVATE ${SDL2_LIBRARIES} ${SDL2TTF_LIBRARIES} ${SDL2IMAGE_LIBRARIES} libLibElysianVMU)

target_include_directories(vmuPI PRIVATE include)

