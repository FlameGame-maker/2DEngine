cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(2DEngine)

add_executable(
	2DEngine src/main.cpp
	src/Renderer/basicShader.cpp
	src/Renderer/basicShader.h
)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
target_link_libraries(2DEngine glfw)
target_link_libraries(2DEngine glad)
find_package(OpenGL REQUIRED)
target_link_libraries(2DEngine OpenGL::GL)
