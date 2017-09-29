cmake_minimum_required(VERSION 3.1.3)

project("OpenGL")

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMakeModules/")

if(WIN32)
	list(APPEND CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/Dependencies/")
endif()

message("CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

# This is useful for versioning. This declaration sets version 1.0.
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(VERSION_STRING "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE DEBUG)
endif()

set(CMAKE_CXX_STANDARD 14)

message("CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")

if(CMAKE_BUILD_TYPE MATCHES DEBUG)
	add_definitions(-DDEBUG -DDEBUG_SHADER -DPROFILE)
elseif(CMAKE_BUILD_TYPE MATCHES FINAL_RELEASE) # Release: Clean up your shit edition
	add_definitions(-DNO_PROFILE -DNO_DEBUG)
	set(CMAKE_BUILD_TYPE RELEASE)
endif()

# Configuration file to pump CMake variables into the project
configure_file(
	"Source/CMakeConfig.h.in"
	"${PROJECT_BINARY_DIR}/CMakeConfig.h"
)

if(WIN32)
	include_directories("${PROJECT_SOURCE_DIR}/Dependencies/include/")
	link_directories("${PROJECT_SOURCE_DIR}/Dependencies/lib/")

	set(SDL2_PATH "${PROJECT_SOURCE_DIR}/Dependencies/SDL2-2.0.5/lib/x64")

endif()

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(GLEW REQUIRED)
#find_package(Freetype REQUIRED)

set(LIBS
	${OPENGL_LIBRARIES}
#	${GLEW_LIBRARIES}
#	${FREETYPE_LIBRARIES}
	${SDL2_LIBRARY}
)

if(WIN32)
	list(APPEND LIBS "mingw32")
endif()

include_directories(
	${OPENGL_INCLUDE_DIRS}
#	${GLEW_INCLUDE_DIRS}
#	${FREETYPE_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIR}

	# For CMakeConfig.h
	${PROJECT_BINARY_DIR}
)

set(Sources
	# Where the main() function is stored
	Source/Main.cpp

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
	Source/GUI/GUIPrim.cpp
	Source/GUI/GUIRenderer.cpp

	# Physics code
	Source/Physics/AABB.cpp
	Source/Physics/ColWrapper.cpp
	# Source/Physics/BoundingSphere.cpp # Deprecated due to no visible use
	Source/Physics/CheckCollision.cpp
)

add_executable(${PROJECT_NAME} ${Sources})

target_link_libraries(${PROJECT_NAME} ${LIBS} ${CMAKE_DL_LIBS})
#End work here
