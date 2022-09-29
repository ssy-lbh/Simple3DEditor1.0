# CMakeList.txt: Simple3DEditor 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

set(TARGET_NAME Simple3DEditor)
set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/")
set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_PATH}/${TARGET_NAME}/")
set(LIBRARY_OUTPUT_PATH "${OUTPUT_PATH}//${TARGET_NAME}/")  

file(COPY lib/glut/glut64.dll DESTINATION "${EXECUTABLE_OUTPUT_PATH}/Debug/")
file(COPY lib/glew/glew32.dll DESTINATION "${EXECUTABLE_OUTPUT_PATH}/Debug/")

file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE EDITOR_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE EDITOR_RESOURCE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)

# 将源代码添加到此项目的可执行文件。
link_directories(
	lib/glut
	lib/openal
	lib/glew
)

add_executable (${TARGET_NAME} ${EDITOR_HEADERS} ${EDITOR_SOURCES} ${EDITOR_RESOURCE})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Simple3DEditor PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标.
include_directories(AFTER ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(
	${TARGET_NAME}
	glut64
	glew32
	OpenAL32
)
