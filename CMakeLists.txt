cmake_minimum_required(VERSION 3.2)
# add_definitions(-std=c++11)
add_definitions("-Wall -g")
add_definitions("-DRPC_SVC_FG")
# add_definitions("-lglib-2.0")
# 添加链接库文件目录 LINK_DIRECTORIES
# link_libraries(`pkg-config --cflags glib-2.0`)
link_libraries(glib-2.0)
# 设置编译器及优先搜索目录
set(CMAKE_FIND_ROOT_PATH /usr/lib)
set(CMAKE_CXX_COMPILER /usr/bin/c++)
set(CMAKE_C_COMPILER /usr/bin/cc)

include_directories("/usr/local/include/")
include_directories("/usr/local/include/glib-2.0")
link_directories("/usr/local/lib")

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
add_subdirectory(src/webinterface)

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib/)
