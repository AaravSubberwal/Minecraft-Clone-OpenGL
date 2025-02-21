cmake_minimum_required(VERSION 3.10)
project(MinecraftCloneOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set default build type if not specified
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the build type" FORCE)
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2")
endif()

set(SOURCES
    src/main.cpp
    src/Renderer.cpp
    src/shader.cpp
    src/Minecraft.cpp
    src/textures.cpp
    src/Camera.cpp
    src/Window.cpp
    src/vendor/stb_image.cpp
    src/vendor/glad.c
)

add_executable(cutable ${SOURCES})

target_include_directories(cutable PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/vendor
)
target_compile_options(cutable PRIVATE -O2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Platform-specific linking
if (WIN32)
    target_compile_definitions(cutable PRIVATE GLFW_STATIC)
    target_link_directories(cutable PRIVATE ${PROJECT_SOURCE_DIR}/lib)
    target_link_libraries(cutable glfw3 opengl32 gdi32)
elseif (UNIX AND NOT APPLE)
    find_package(OpenGL REQUIRED)
    find_package(GLFW REQUIRED)
    target_link_libraries(cutable glfw OpenGL::GL dl pthread)
elseif (APPLE)
    find_library(OPENGL_LIBRARY OpenGL)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(cutable ${OPENGL_LIBRARY} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
endif()
