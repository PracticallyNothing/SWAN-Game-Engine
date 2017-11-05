cmake_minimum_required(VERSION 3.1.3)

project("SWAN")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")

if(WIN32)
	list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/Dependencies/")
endif()

message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# Creates a compile_commands.json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Versioning
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE SWAN_DEBUG)
endif()

set(CMAKE_CXX_STANDARD 14)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_definitions(-DSWAN_DEBUG -DSWAN_DEBUG_SHADER -DSWAN_PROFILE)
elseif(CMAKE_BUILD_TYPE MATCHES PROFILE)
	add_definitions(-DSWAN_PROFILE)
	list(APPEND CMAKE_CXX_FLAGS "-pg")
	set(CMAKE_BUILD_TYPE RELEASE)
elseif(CMAKE_BUILD_TYPE MATCHES FINAL_RELEASE) # Release: Clean up your stuff edition
	add_definitions(-DSWAN_NO_PROFILE -DSWAN_NO_DEBUG)
	set(CMAKE_BUILD_TYPE RELEASE)
endif()

# Configuration file to pump CMake variables into the project
configure_file(
	"Source/CMakeConfig.h.in"
	"${PROJECT_BINARY_DIR}/CMakeConfig.h"
)

include_directories("${PROJECT_SOURCE_DIR}/Source/")
include_directories("${PROJECT_SOURCE_DIR}/Source/External/cpptoml/include/")
include_directories("${PROJECT_SOURCE_DIR}/Dependencies/include/")

if(WIN32)
	link_directories("${PROJECT_SOURCE_DIR}/Dependencies/lib/")

	set(SDL2_PATH "${PROJECT_SOURCE_DIR}/Dependencies/SDL2-2.0.5/lib/x64")
endif()

if(LINUX)
	add_compile_options("-ftest-coverage")
endif()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(Freetype REQUIRED)

set(LIBS
	${OPENGL_LIBRARIES}
#	${FREETYPE_LIBRARIES}
	${SDL2_LIBRARY}
)

include_directories(
	${OPENGL_INCLUDE_DIRS}
#	${FREETYPE_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIR}

	# For CMakeConfig.h
	${PROJECT_BINARY_DIR}
)

set(Headers
	Source/Core/*.hpp
	Source/GUI/*.hpp
	Source/Logic/*.hpp
	Source/Physics/*.hpp
	Source/Rendering/*.hpp
	Source/Utility/*.hpp
	"${PROJECT_BINARY_DIR}/CMakeConfig.h"
)

set(Sources
	# Replacement for GLEW
	Source/External/glad/src/glad.c

	# Core files for the engine
	Source/Core/Display.cpp
	Source/Core/Resources.cpp
	Source/Core/Input.cpp

	# Utility code (parsers, maths, debugging, etc.)
	Source/Utility/XML.cpp
	Source/Utility/StringUtil.cpp

	# Rendering code
	Source/Rendering/Texture.cpp
	Source/Rendering/BitmapFont.cpp
	Source/Rendering/Image.cpp
	Source/Rendering/Mesh.cpp
	Source/Rendering/Shader.cpp
	Source/Rendering/Text.cpp
	Source/Rendering/OBJ-Import.cpp

	# GUI stuff
	Source/GUI/Element.cpp
	Source/GUI/Renderer.cpp

	# Physics code
	Source/Physics/AABB.cpp
	Source/Physics/ColWrapper.cpp
	# Source/Physics/BoundingSphere.cpp
	Source/Physics/CheckCollision.cpp
)

add_library(SWAN STATIC ${Sources})
target_link_libraries(SWAN ${LIBS} ${CMAKE_DL_LIBS})

add_executable(SWAN-Unnamed-Demo Demos/SWAN-Unnamed-Demo.cpp)
target_link_libraries(SWAN-Unnamed-Demo SWAN)

add_executable(SWAN-GUI-Demo Demos/SWAN-GUI-Demo.cpp)
target_link_libraries(SWAN-GUI-Demo SWAN)

add_executable(SWAN-Text-Demo Demos/SWAN-Text-Demo.cpp)
target_link_libraries(SWAN-Text-Demo SWAN)

install(FILES ${Headers} DESTINATION include)
install(TARGETS SWAN ARCHIVE DESTINATION lib)
