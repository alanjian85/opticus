cmake_minimum_required(VERSION 3.0)
project(rtow)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

add_executable(rtow 
    deps/glad/src/glad.c
    src/main.cpp
)

target_link_libraries(rtow PUBLIC
    glfw
    glm
)

target_include_directories(rtow PUBLIC
    deps/glad/include
    deps/stb
)

target_compile_definitions(rtow PUBLIC
    GLFW_INCLUDE_NONE
)