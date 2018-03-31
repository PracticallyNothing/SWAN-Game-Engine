cmake_minimum_required(VERSION 3.1.3)
project("SWAN")

# Versioning
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
	"CMakeConfig.h.in"
	"${PROJECT_BINARY_DIR}/CMakeConfig.h"
)

include_directories("${PROJECT_SOURCE_DIR}/Source/External/cpptoml/include/")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

message("SWAN:")
message("    -> SDL2_INCLUDE_DIRS: ${SDL2_INCLUDE_DIRS}")
message("    -> OPENGL_INCLUDE_DIRS: ${OPENGL_INCLUDE_DIRS}")
message("    -> SDL2_LIBRARIES: ${SDL2_LIBRARIES}")
message("    -> OPENGL_LIBRARIES: ${OPENGL_LIBRARIES}")

set(LIBS
	${OPENGL_LIBRARIES}
	${SDL2_LIBRARY}
)

include_directories(
	${OPENGL_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIR}

	# For CMakeConfig.h
	${PROJECT_BINARY_DIR}
	${PROJECT_SOURCE_DIR}
)

set(Headers
	Core/*.hpp
	GUI/*.hpp
	Logic/*.hpp
	Physics/*.hpp
	Rendering/*.hpp
	Utility/*.hpp
	"${PROJECT_BINARY_DIR}/CMakeConfig.h"
)

set(Sources
	# Replacement for GLEW
	External/glad/src/glad.c

	# Core files for the engine
	Core/Display.cpp
	Core/Resources.cpp
	Core/Input.cpp

	# OpenGL rendering code
	OpenGL/VAO.cpp

	# Utility code (parsers, maths, debugging, etc.)
	Utility/XML.cpp
	Utility/StringUtil.cpp
	Utility/Octree.cpp

	# Rendering code
	Rendering/Texture.cpp
	Rendering/BitmapFont.cpp
	Rendering/Image.cpp
	Rendering/Mesh.cpp
	Rendering/Shader.cpp
	Rendering/Text.cpp
	Rendering/OBJ-Import.cpp

	# GUI stuff
	GUI/Element.cpp
	GUI/Renderer.cpp

	# Physics code
	Physics/AABB.cpp
	Physics/ColWrapper.cpp
	# Physics/BoundingSphere.cpp
	Physics/CheckCollision.cpp
)

add_library(SWAN STATIC ${Sources})
target_link_libraries(SWAN ${LIBS} ${CMAKE_DL_LIBS})

install(FILES ${Headers} DESTINATION include)
install(TARGETS SWAN ARCHIVE DESTINATION lib)
