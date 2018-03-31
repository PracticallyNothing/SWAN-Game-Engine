cmake_minimum_required(VERSION 3.1.3)
project("SWAN FPS")

set(SOURCES
	Main.cpp)

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(LIBS
  SWAN
  ${OPENGL_gl_LIBRARIES}
  ${SDL2_LIBRARIES})

add_executable(SWAN-FPS-Demo ${SOURCES})
target_link_libraries(SWAN-FPS-Demo ${LIBS})
