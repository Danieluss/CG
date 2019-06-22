cmake_minimum_required(VERSION 3.12)
project(CG)

MATH(EXPR stack_size "512 * 1024 * 1024") # 512 MB
set(CMAKE_EXE_LINKER_FLAGS "-Wl,--stack,${stack_size}")
set(CMAKE_CXX_STANDARD 11)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -I./src")

file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/lib/glew/*.c
        ${CMAKE_SOURCE_DIR}/lib/glew/*.cpp
        ${CMAKE_SOURCE_DIR}/src/*.c
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/src/*.h
        ${CMAKE_SOURCE_DIR}/src/*.hpp)

include_directories(${CMAKE_BINARY_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/lib/glm)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/lib/assimp)

find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(Assimp REQUIRED)

include_directories(${ASSIMP_INCLUDE_DIRS})
link_libraries(${ASSIMP_LIBRARIES})

include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})

include_directories(${OPENGL_INCLUDE_DIRS})
link_libraries(${OPENGL_LIBRARIES})

add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/entities/basic_entities.cpp src/entities/basic_entities.hpp)

target_link_libraries(CG -L -lglew32s -lglfw3dll)
