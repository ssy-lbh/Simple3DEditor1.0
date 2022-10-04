# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project("Simple3DEditor" VERSION 1.0.0)

set(OS_PLATFORM windows)

# 主项目
file(GLOB_RECURSE PLATFORM_SOURCES_CPP CONFIGURE_DEPENDS
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/${OS_PLATFORM}/*.cpp)

add_subdirectory(platform)
add_subdirectory(src)

# 包含子项目。
