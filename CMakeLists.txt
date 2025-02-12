cmake_minimum_required(VERSION 3.10)
project(MinecraftCloneOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    src/main.cpp
    src/GLbuffers.cpp
    src/Renderer.cpp
    src/shader.cpp
    src/textures.cpp
    src/vendor/stb_image.cpp
    src/vendor/glad.c
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/src/vendor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
add_executable(cutable ${SOURCES})

if (WIN32)
    target_compile_definitions(cutable PRIVATE GLFW_STATIC)
    target_link_directories(cutable PRIVATE ${PROJECT_SOURCE_DIR}/lib)
    target_link_libraries(cutable glfw3 opengl32 gdi32)
endif()

if (UNIX AND NOT APPLE)
    target_link_libraries(cutable glfw GL X11 pthread Xrandr Xi dl)
endif()

if (APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(cutable ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif()
