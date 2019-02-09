cmake_minimum_required(VERSION 3.1.3)
project("SWAN Full Project")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "./CMakeModules/")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

if(WIN32)
  list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/../Dependencies/")
endif()

# Creates a compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/SWAN/")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/") # For GLAD

if(WIN32)
  include_directories("${PROJECT_SOURCE_DIR}/Dependencies/include")
  link_directories("${PROJECT_SOURCE_DIR}/Dependencies/lib/")
  set(SDL2_PATH "${PROJECT_SOURCE_DIR}/Dependencies/SDL2-2.0.5/lib/x64")
endif()

add_subdirectory(SWAN)
add_subdirectory(FPS)
add_subdirectory(Demos)
add_subdirectory(Tools)

