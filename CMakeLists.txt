cmake_minimum_required(VERSION 3.17)
project(CG_Project)

set(CMAKE_CXX_STANDARD 11)

#glfw
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
add_subdirectory(External/glfw-master)
include_directories(External/glfw-master/include)
LIST(APPEND LIBS glfw ${GLFW_LIBRARIES})

#glad
set(GLAD_SRC External/glad-c/src/glad.c)
if(NOT_WIN32)
    LIST(APPEND LIBS dl)
endif()
include_directories(External/glad-c/include)

#glm
include_directories(External/glm-0.9.9.7)

#assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(INJECT_DEBUG_POSTFIX OFF)  #on if debug
add_subdirectory(External/assimp-5.0.1)
include_directories(External/assimp-5.0.1/include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/External/assimp-5.0.1/include)
LIST(APPEND LIBS assimp)

#ours
include_directories(include)


set(SOURCE_FILES main.cpp ${GLAD_SRC} )
add_executable(CG_Project ${SOURCE_FILES})
target_link_libraries(CG_Project ${LIBS})