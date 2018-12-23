file(GLOB sources
  "rpc_clnt.c"
  "rpc_call_fun.c"
  )
link_directories("${CMAKE_BINARY_DIR}/lib")
include_directories("../common")

option(BUILD_SHARED_LIBS "Build shared libraries DLLs." ON)

# 使用宏判断是否编译二进制，默认编译
option(BUILD_BINARY_EXE "Build binary." ON)
if (BUILD_BINARY_EXE)
  add_definitions(-DBUILD_BINARY)
  message(STATUS "Build rpc client BINARY.")
else()
  message(STATUS "Only build dynamic library.")
endif()

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

add_library(rpc_clnt SHARED ${sources})
add_executable(client rpc_client.c)
target_link_libraries(rpc_clnt com)
target_link_libraries(client rpc_clnt)
