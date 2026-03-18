cmake_minimum_required(VERSION 3.20)
project(opengl-sandbox-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

add_executable(opengl-sandbox src/main.cpp)

target_compile_definitions(opengl-sandbox PRIVATE GL_SILENCE_DEPRECATION)

target_include_directories(opengl-sandbox PRIVATE
    /Users/defv/dev/libs/glfw-3.4/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_directories(opengl-sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/../build/src
)

target_link_libraries(opengl-sandbox PRIVATE
    glfw3
    OpenGL::GL
    "-framework Cocoa"
    "-framework IOKit"
)
