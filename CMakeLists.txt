cmake_minimum_required(VERSION 3.10)


add_subdirectory(opt)
project(PegasosEngine)


find_package(Vulkan REQUIRED)

add_subdirectory(Window)
add_subdirectory(Renderer)

add_executable(PegasosApp main.cpp)

target_link_libraries(PegasosApp Window VulkanRenderer)