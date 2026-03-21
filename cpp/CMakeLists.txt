cmake_minimum_required(VERSION 3.20)
project(opengl-sandbox-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenGL REQUIRED)

# Support one extra directory level under src/bin (e.g. src/bin/triangle/*.cpp),
# while keeping compatibility with existing flat files.
file(GLOB EXAMPLE_SOURCES
    "src/bin/*.cpp"
    "src/bin/*/*.cpp"
)

foreach(SOURCE ${EXAMPLE_SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    file(RELATIVE_PATH SOURCE_REL_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/bin" "${SOURCE}")
    get_filename_component(SOURCE_SUBDIR "${SOURCE_REL_PATH}" DIRECTORY)

    if(SOURCE_SUBDIR STREQUAL "." OR SOURCE_SUBDIR STREQUAL "")
        set(TARGET_ID "${TARGET_NAME}")
    else()
        string(REPLACE "/" "_" TARGET_ID "${SOURCE_SUBDIR}_${TARGET_NAME}")
    endif()

    add_executable(${TARGET_ID} ${SOURCE} src/glad.c src/utils.cpp)
    set_target_properties(${TARGET_ID} PROPERTIES OUTPUT_NAME "${TARGET_NAME}")

    if(SOURCE_SUBDIR STREQUAL "." OR SOURCE_SUBDIR STREQUAL "")
        set_target_properties(${TARGET_ID} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
        )
    else()
        set_target_properties(${TARGET_ID} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${SOURCE_SUBDIR}"
        )
    endif()

    target_compile_definitions(${TARGET_ID} PRIVATE GL_SILENCE_DEPRECATION)
    target_include_directories(${TARGET_ID} PRIVATE
        /Users/defv/dev/libs/glfw-3.4/include
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    )
    target_link_directories(${TARGET_ID} PRIVATE
        ${CMAKE_SOURCE_DIR}/../build/src
    )
    target_link_libraries(${TARGET_ID} PRIVATE
        glfw3
        OpenGL::GL
        "-framework Cocoa"
        "-framework IOKit"
    )
endforeach()
