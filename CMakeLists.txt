cmake_minimum_required(VERSION 3.1.3)
project("SWAN + Demos + FPS")

#if(UNIX)
#	set(CMAKE_C_COMPILER "clang")
#	set(CMAKE_CXX_COMPILER "clang++")
#	list(APPEND CMAKE_CXX_FLAGS "-fsanitize=undefined")
#endif()

if(WIN32)
	list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/../Dependencies/")
endif()

list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/../Dependencies/")

# Creates a compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/SWAN/")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/include")

if(WIN32)
	link_directories("${PROJECT_SOURCE_DIR}/Dependencies/lib/")
	set(SDL2_PATH "${PROJECT_SOURCE_DIR}/Dependencies/SDL2-2.0.5/lib/x64")
endif()

add_subdirectory(SWAN)
add_subdirectory(FPS)
add_subdirectory(Demos)
