cmake_minimum_required(VERSION 3.20)
project(opengl-sandbox-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

file(GLOB EXAMPLE_SOURCES "src/bin/*.cpp")

foreach(SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE} src/glad.c)
    target_compile_definitions(${TARGET_NAME} PRIVATE GL_SILENCE_DEPRECATION)
    target_include_directories(${TARGET_NAME} PRIVATE
        /Users/defv/dev/libs/glfw-3.4/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_directories(${TARGET_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/../build/src
    )
    target_link_libraries(${TARGET_NAME} PRIVATE
        glfw3
        OpenGL::GL
        "-framework Cocoa"
        "-framework IOKit"
    )
endforeach()
