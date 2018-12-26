cmake_minimum_required(VERSION 3.1.3)
project("SWAN FPS")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "../CMakeModules/")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE DEBUG)
endif()

set(SOURCES
  Main.cpp)

#set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
#find_package(assimp REQUIRED)

set(LIBS
  SWAN
  ${OPENGL_gl_LIBRARIES}
  ${SDL2_LIBRARIES}
  ${ASSIMP_LIBRARIES}
  )

#add_executable(SWAN-FPS-Demo ${SOURCES})
#target_link_libraries(SWAN-FPS-Demo ${LIBS})
