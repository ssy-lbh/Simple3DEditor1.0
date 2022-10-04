# CMakeList.txt: Simple3DEditor 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
set(TARGET_NAME Simple3DEditor)
set(OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../build/")
set(LIBRARY_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lib/")
set(EXECUTABLE_OUTPUT_PATH "${OUTPUT_PATH}/${TARGET_NAME}/")
set(LIBRARY_OUTPUT_PATH "${OUTPUT_PATH}/${TARGET_NAME}/")  

file(COPY ${LIBRARY_PATH}/glut/glut64.dll DESTINATION "${EXECUTABLE_OUTPUT_PATH}/Debug/")
file(COPY ${LIBRARY_PATH}/glew/glew32.dll DESTINATION "${EXECUTABLE_OUTPUT_PATH}/Debug/")

file(GLOB_RECURSE EDITOR_HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB_RECURSE EDITOR_SOURCES_C CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.c)
file(GLOB_RECURSE EDITOR_SOURCES_CPP CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE EDITOR_RESOURCE CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.rc)

add_subdirectory(base)
add_subdirectory(editor)
add_subdirectory(io)
add_subdirectory(lib)
add_subdirectory(manager)
add_subdirectory(res)
add_subdirectory(util)

add_executable(${TARGET_NAME} ${PLATFORM_SOURCES_CPP}
	${EDITOR_HEADERS} ${EDITOR_SOURCES_C} ${EDITOR_SOURCES_CPP} ${EDITOR_RESOURCE})

# 将源代码添加到此项目的可执行文件。
# lib文件夹应该为项目内代码，其他需单独编译的项目应置于根目录其他位置
target_link_directories(${TARGET_NAME} PUBLIC ${LIBRARY_PATH})

target_include_directories(
	${TARGET_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lib
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../lib
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../imgui
)

target_link_libraries(
	${TARGET_NAME}
	opengl32
	glu32
	glut/glut64
	glew/glew32
	openal/OpenAL32
	glfw/glfw3
)

if(CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${TARGET_NAME} PROPERTY CXX_STANDARD 20)
endif()
