cmake_minimum_required(VERSION 3.9)
project(Renderer)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(tinyobjloader)
add_subdirectory(glfw-3.2.1)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
#include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(Renderer main.cpp scene/Scene.h scene/Scene.cpp scene/Renderer.cpp scene/Renderer.h mathdebug.cpp mathdebug.h gl/shaders.h gl/shaders.cpp gl/RenderTexture.cpp gl/RenderTexture.h)
target_link_libraries(Renderer tinyobjloader)
target_link_libraries(Renderer ${OPENGL_LIBRARIES})
target_link_libraries(Renderer glfw)
target_link_libraries(Renderer GLEW)