cmake_minimum_required(VERSION 3.20)
project(flower VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find SDL3
find_package(SDL3 QUIET)

if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found, using FetchContent to download it")
    include(FetchContent)
    
    # Note: In headless/CI environments, SDL3 may not build with video support
    # For development, install X11/Wayland libraries as per SDL3 documentation
    
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG main
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    # Configure SDL3 options
    set(SDL_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_STATIC ON CACHE BOOL "" FORCE)
    set(SDL_TEST OFF CACHE BOOL "" FORCE)
    # Allow building without X11/Wayland for CI/headless environments
    set(SDL_X11_SHARED OFF CACHE BOOL "" FORCE)
    set(SDL_WAYLAND_SHARED OFF CACHE BOOL "" FORCE)
    
    FetchContent_MakeAvailable(SDL3)
endif()

# Source files
set(SOURCES
    src/main.cpp
    src/player.cpp
    src/engine.cpp
    src/weapon.cpp
    src/pickup.cpp
    src/limb.cpp
)

set(HEADERS
    src/player.h
    src/engine.h
    src/weapon.h
    src/pickup.h
    src/limb.h
    src/math_utils.h
)

# Create executable
add_executable(flower ${SOURCES} ${HEADERS})

# Link SDL3
target_link_libraries(flower PRIVATE SDL3::SDL3)

# Include directories
target_include_directories(flower PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Platform-specific settings
if(WIN32)
    target_link_libraries(flower PRIVATE opengl32)
elseif(APPLE)
    target_link_libraries(flower PRIVATE "-framework OpenGL")
else()
    target_link_libraries(flower PRIVATE GL)
endif()

# Set output directory
set_target_properties(flower PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
