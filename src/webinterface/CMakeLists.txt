file(GLOB sources
  "*.c")

link_directories("${CMAKE_BINARY_DIR}/lib")
option(BUILD_SHARED_LIBS ON)
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
add_library(rpc_web SHARED ${sources})
target_link_libraries(rpc_web rpc_clnt)
