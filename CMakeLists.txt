cmake_minimum_required(VERSION 3.2)
# add_definitions(-std=c++11)
add_definitions("-Wall -g")
add_definitions("-DRPC_SVC_FG")
# 添加链接库文件目录 LINK_DIRECTORIES
link_libraries(glib-2.0.0)
# link_libraries(`pkg-config --cflags glib-2.0`)

include_directories("/usr/local/include/glib-2.0")

PROJECT(RPCSample)
set(RPCSample_version 1.0.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE on)
# add compile flags to cxx
#set(CMAKE_CXX_FLAGS "")
include_directories(src/common)

# include(CMakeDependentOption)
add_subdirectory(src/common)
add_subdirectory(src/apps)
add_subdirectory(src/smp)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/)
