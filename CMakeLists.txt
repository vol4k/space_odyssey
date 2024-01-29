cmake_minimum_required(VERSION 3.13.0)

set(PROJECT_NAME opengl_space_odyssey)

set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs)
set(
  SOURCES

  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/Camera.cpp
  ${SRC_DIR}/ex_8_1.hpp
  ${SRC_DIR}/ex_8_2.hpp
  ${SRC_DIR}/Render_Utils.h
)

project(${PROJECT_NAME} VERSION 0.0.1)

add_subdirectory(libs/glfw)
add_subdirectory(libs/glew)
add_subdirectory(libs/glm)
add_subdirectory(libs/freeglut)
add_subdirectory(libs/assimp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR}/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR}/glew/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR}/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR}/freeglut/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${LIB_DIR}/assimp/include)

file(COPY ${BUILD_DIR}/include/assimp/config.h DESTINATION ${LIB_DIR}/assimp/include/assimp/)
file(COPY ${BUILD_DIR}/revision.h DESTINATION ${LIB_DIR}/assimp/include/assimp/)

target_include_directories(${PROJECT_NAME} PRIVATE ${SRC_DIR})