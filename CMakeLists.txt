cmake_minimum_required(VERSION 3.5)
project(opticus)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(opticus
    deps/glad/src/glad.c
    src/stb.c
    src/main.cpp
    src/ScreenMesh.cpp
    src/Program.cpp
    src/Cubemap.cpp
    src/Camera.cpp
    src/Shader.cpp
)

find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

target_link_libraries(opticus PUBLIC
    glfw
)

target_include_directories(opticus PUBLIC
    deps/glad/include
    deps/stb
    include
)

target_compile_definitions(opticus PUBLIC
    GLFW_INCLUDE_NONE
)
