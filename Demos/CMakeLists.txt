cmake_minimum_required(VERSION 3.1.3)
project("SWAN Demos")

find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(LIBS
  SWAN
  ${OPENGL_gl_LIBRARIES}
  ${SDL2_LIBRARIES})

add_executable(SWAN-Cubes-Demo SWAN-Cubes-Demo.cpp)
target_link_libraries(SWAN-Cubes-Demo ${LIBS})

add_executable(SWAN-GUI-Demo SWAN-GUI-Demo.cpp)
target_link_libraries(SWAN-GUI-Demo ${LIBS})

add_executable(SWAN-Unnamed-Demo SWAN-Unnamed-Demo.cpp)
target_link_libraries(SWAN-Unnamed-Demo ${LIBS})
