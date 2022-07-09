cmake_minimum_required(VERSION 3.0)
project(rtnow)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)

add_executable(rtnow 
    deps/glad/src/glad.c
    src/main.cpp
    src/ScreenQuad.cpp
)

target_link_libraries(rtnow PUBLIC
    glfw
    glm
)

target_include_directories(rtnow PUBLIC
    deps/glad/include
    deps/stb
    include
)

target_compile_definitions(rtnow PUBLIC
    GLFW_INCLUDE_NONE
)