cmake_minimum_required(VERSION 3.1.3)
project("SWAN Tools")

#set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

set(LIBS
  SWAN
  ${OPENGL_gl_LIBRARIES}
  ${SDL2_LIBRARIES}
  )

add_executable(ImageToHpp ImageToHpp.cpp)
target_link_libraries(ImageToHpp ${LIBS})

add_executable(ShaderReader ShaderReader.cpp)
target_link_libraries(ShaderReader ${LIBS})
