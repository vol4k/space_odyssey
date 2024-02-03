cmake_minimum_required(VERSION 3.14.0)

project(opengl_space_odyssey)

project(${PROJECT_NAME} VERSION 0.0.4 LANGUAGES CXX)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SOURCES 
  ${SRC_DIR}/main.cpp
  ${SRC_DIR}/OWindow.cpp
  ${SRC_DIR}/OGame.cpp
  ${SRC_DIR}/OGraphicsEngine.cpp
  ${SRC_DIR}/OResourceUnit.cpp
  ${SRC_DIR}/OShaderUnit.cpp
  ${SRC_DIR}/OGameObject.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E rm -rf  
    ${PROJECT_BINARY_DIR}/res
    COMMENT "Remove resources from binary directory"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
    ${PROJECT_SOURCE_DIR}/res 
    ${PROJECT_BINARY_DIR}/res
    COMMENT "Copying resources into binary directory"
)

# New in 3.26
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory_if_different 
#    ${PROJECT_SOURCE_DIR}/res 
#    ${PROJECT_BINARY_DIR}/res
#    COMMENT "Update resources if it change"
#)


add_subdirectory(deps/glew)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glm)
add_subdirectory(deps/freeglut)
add_subdirectory(deps/assimp)
add_subdirectory(deps/soil2)

target_include_directories(${PROJECT_NAME} 
  PUBLIC deps/glew/include
  PUBLIC deps/glfw/include
  PUBLIC deps/glm/glm
  PUBLIC deps/freeglut/include
  PUBLIC deps/assimp/include
  PUBLIC deps/soil2/src
)

target_link_directories(${PROJECT_NAME}
  PRIVATE deps/glew/lib
  PRIVATE deps/glfw/src
  PRIVATE deps/glm/glm
  PRIVATE deps/freeglut/lib
  PRIVATE deps/assimp/lib
  PRIVATE deps/soil2
)

target_link_libraries(${PROJECT_NAME} glfw libglew_shared glm freeglut assimp soil2)