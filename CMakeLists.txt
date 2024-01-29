cmake_minimum_required(VERSION 3.14.0)

project(opengl_space_odyssey)

project(${PROJECT_NAME} VERSION 0.0.2 LANGUAGES CXX)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES 
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/OWindow.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(deps/glew)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/freeglut)
#add_subdirectory(deps/assimp)

target_include_directories(${PROJECT_NAME} 
  PUBLIC deps/glew/include
  PUBLIC deps/glfw/include
  PUBLIC deps/glm/glm
  PUBLIC deps/freeglut/include
  #PUBLIC deps/assimp/include
)

target_link_directories(${PROJECT_NAME}
  PRIVATE deps/glew/lib
  PRIVATE deps/glfw/src
  PRIVATE deps/glm/glm
  PRIVATE deps/freeglut/lib
  #PRIVATE deps/assimp/lib
)

target_link_libraries(${PROJECT_NAME} glfw libglew_shared glm freeglut)# assimp)