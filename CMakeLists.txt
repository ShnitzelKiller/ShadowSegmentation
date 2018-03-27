cmake_minimum_required(VERSION 3.9)
project(Renderer)

set(CMAKE_CXX_STANDARD 11)
add_subdirectory(tinyobjloader)
add_subdirectory(glfw-3.2.1)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenCV REQUIRED)
#include_directories(${OPENGL_INCLUDE_DIRS})

add_executable(Renderer main.cpp scene/Scene.h scene/Scene.cpp scene/Renderer.cpp scene/Renderer.h mathdebug.cpp mathdebug.h gl/shaders.h gl/shaders.cpp gl/RenderTexture.cpp gl/RenderTexture.h overlays/ScreenspaceQuad.cpp overlays/ScreenspaceQuad.h scene/ParallelSceneRenderer.cpp scene/ParallelSceneRenderer.h overlays/TextureQuad.cpp overlays/TextureQuad.h overlays/DilateQuad.cpp overlays/DilateQuad.h overlays/PointLitQuad.cpp overlays/PointLitQuad.h overlays/DirLitQuad.cpp overlays/DirLitQuad.h gl/RenderContext.cpp gl/RenderContext.h)
target_link_libraries(Renderer tinyobjloader)
target_link_libraries(Renderer ${OPENGL_LIBRARIES})
target_link_libraries(Renderer glfw)
target_link_libraries(Renderer GLEW)
target_link_libraries(Renderer ${OpenCV_LIBS})