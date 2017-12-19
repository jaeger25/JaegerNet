# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project ("JaegerNet")

set(CMAKE_CXX_STANDARD 17)

set(PROTOBUF_PATH "Dependencies/protobuf-3.5.0" )
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} "${PROTOBUF_PATH}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")
#set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${PROTOBUF_PATH}/lib")

include(cotire)
include(FindProtobuf)
find_package(Protobuf CONFIG REQUIRED)

add_definitions (-DASIO_STANDALONE -DASIO_HAS_STD_SHARED_PTR -DASIO_HAS_STD_ADDRESSOF -DASIO_HAS_STD_ARRAY -DASIO_HAS_STD_TYPE_TRAITS -DASIO_HAS_CSTDINT)

include_directories(${CMAKE_BINARY_DIR}/proto)
include_directories(Dependencies/asio-1.10.6/include)
include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(inc)

# Include sub-projects.
add_subdirectory ("Client")
add_subdirectory ("Proto")
add_subdirectory ("Server")