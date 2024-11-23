cmake_minimum_required(VERSION 3.29)
project(Solar)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

if (WIN32)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})
endif ()

include_directories(include)

# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/external/glfw)
include_directories(${CMAKE_SOURCE_DIR}/external/glfw/include)

# GLAD
add_library(glad src/glad.c)
include_directories(include/glad)

# GLM
include_directories(${CMAKE_SOURCE_DIR}/external/glm)

add_executable(SolarSystem2 src/main.cpp
        include/Shader.h
        src/Shader.cpp
        src/Sphere.cpp
        include/Sphere.h
        src/Camera.cpp
        include/Camera.h
        src/stb_image.cpp
        include/Skybox.h
        src/Skybox.cpp
)

target_link_libraries(SolarSystem2 PRIVATE glfw glad)
target_include_directories(SolarSystem2 PRIVATE include)
target_link_directories(SolarSystem2 PRIVATE ${CMAKE_SOURCE_DIR}/external)
