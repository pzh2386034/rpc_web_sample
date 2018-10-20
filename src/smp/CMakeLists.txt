file(GLOB sources
  "rpc_clnt.c"
  )
link_directories("${CMAKE_BINARY_DIR}/lib")
include_directories("../common")

option(BUILD_SHARED_LIBS "Build shared libraries DLLs." ON)

set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")
set(EXECUTABLE_OUTPUT_PATH "${PROJECT_BINARY_DIR}/bin")

add_library(rpc_clnt ${sources})
add_executable(client rpc_client.c)
target_link_libraries(rpc_clnt com)
target_link_libraries(client rpc_clnt)
