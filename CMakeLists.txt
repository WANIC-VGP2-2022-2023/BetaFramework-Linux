cmake_minimum_required(VERSION 3.27)
project(BetaFramework)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER  "clang++")

FILE(GLOB LowLevelAPI_C "LowLevelAPI_C/src/*" )
FILE(GLOB LowLevelAPI_C_include "LowLevelAPI_C/include/*" )
FILE(GLOB LowLevelAPI "LowLevelAPI/src/*" )
FILE(GLOB LowLevelAPI_include "LowLevelAPI/include/*" )

find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

add_library(BetaLow ${LowLevelAPI} ${LowLevelAPI_include} "glfw/include/glad.h" "glfw/src/glad.cpp")

#target_include_directories(LowLevelAPI PRIVATE "LowLevelAPI_C/include/")


target_include_directories(BetaLow PRIVATE "fmodstudioapi20218linux/api/core/inc")
target_include_directories(BetaLow PRIVATE "LowLevelAPI/include/")
target_include_directories(BetaLow PRIVATE "${OPENGL_INCLUDE_DIR}" "glfw/include/" "/usr/include/freetype2")

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} " -O0")

target_link_libraries(BetaLow OpenGL::GL glfw glm Freetype "/home/jasonl/CLionProjects/BetaFramework-master/fmodstudioapi20218linux/api/core/lib/x86_64/libfmod.so")