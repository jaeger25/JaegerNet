# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

project ("JaegerNet")

include(cotire)

add_definitions (-DASIO_STANDALONE -DASIO_HAS_STD_SHARED_PTR -DASIO_HAS_STD_ADDRESSOF -DASIO_HAS_STD_ARRAY -DASIO_HAS_STD_TYPE_TRAITS -DASIO_HAS_CSTDINT)

include_directories(Dependencies/asio-1.10.6/include)

# Include sub-projects.
add_subdirectory ("Client")
add_subdirectory ("Server")