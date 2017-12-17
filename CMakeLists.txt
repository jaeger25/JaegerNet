# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/CMake")

project ("JaegerNet")

include(cotire)

# Include sub-projects.
add_subdirectory ("Client")
add_subdirectory ("Server")