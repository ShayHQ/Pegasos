cmake_minimum_required(VERSION 3.10)


project(PegasosEngine)

add_subdirectory(opt)
add_subdirectory(Common)

find_package(Vulkan REQUIRED)
find_package(OpenGL REQUIRED)
execute_process(COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/shader.vert -o ${CMAKE_BINARY_DIR}/vert.spv)
execute_process(COMMAND glslc ${CMAKE_CURRENT_SOURCE_DIR}/Shaders/shader.frag -o ${CMAKE_BINARY_DIR}/frag.spv)

add_subdirectory(Window)
add_subdirectory(Renderer)

add_executable(PegasosApp main.cpp)

target_link_libraries(PegasosApp Window Renderer)
