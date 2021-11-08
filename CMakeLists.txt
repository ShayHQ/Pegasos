cmake_minimum_required(VERSION 3.10)


add_subdirectory(opt)
project(CrazyEngine)


find_package(Vulkan REQUIRED)

add_subdirectory(Window)

add_executable(CrazyApp main.cpp)

target_link_libraries(CrazyApp Window)