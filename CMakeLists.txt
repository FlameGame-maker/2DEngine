cmake_minimum_required(VERSION 3.13 FATAL_ERROR)
project(2DEngine)

add_executable(
	2DEngine src/main.cpp
	src/Renderer/basicShader.cpp
	src/Renderer/basicShader.h
	src/Resources/Manager.cpp
	src/Resources/Manager.h
)

target_compile_features(2DEngine PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE) 
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
add_subdirectory(external/glad)
target_link_libraries(2DEngine glfw)
target_link_libraries(2DEngine glad)

set_target_properties(2DEngine PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
add_custom_command(
	TARGET 2DEngine POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:2DEngine>/resources)