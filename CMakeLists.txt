cmake_minimum_required(VERSION 3.0.0)
project(opengl_space_odyssey VERSION 0.0.1)
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)

add_executable(opengl_space_odyssey 
  src/main.cpp
  src/preferences.h
  src/lib/glad.h
  src/lib/glad.c
  )

target_link_libraries(opengl_space_odyssey 
  glfw
  OpenGL::GL
  )