cmake_minimum_required(VERSION 3.2.0)

set(PROJECT_NAME opengl_space_odyssey)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(
  SOURCES

  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/preferences.hpp
)

project(${PROJECT_NAME} VERSION 0.0.1)
cmake_policy(SET CMP0072 NEW)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})

# OpenGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

# glfw
set(GLFW_DIR ${LIB_DIR}/glfw)
set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

# glew
set(GLEW_DIR ${LIB_DIR}/glew)
add_library(glew ${GLEW_DIR}/src/glew.c)
target_include_directories(glew PRIVATE ${GLEW_DIR}/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_DIR}/include)
target_link_libraries(${PROJECT_NAME} glew ${CMAKE_DL_LIBS})

# glm
set(GLM_DIR ${LIB_DIR}/glm)
include_directories(${GLM_DIR})