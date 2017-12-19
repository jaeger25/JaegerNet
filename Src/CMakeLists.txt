# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)
project ("JaegerNet")

set(CMAKE_CXX_STANDARD 17)

# cotire
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

include(cotire)
include(FindProtobuf)
find_package(Protobuf CONFIG REQUIRED)

add_definitions (-DASIO_STANDALONE -DASIO_HAS_STD_SHARED_PTR -DASIO_HAS_STD_ADDRESSOF -DASIO_HAS_STD_ARRAY -DASIO_HAS_STD_TYPE_TRAITS -DASIO_HAS_CSTDINT)

add_compile_options(/W4 /WX /wd4251 /wd4996 /wd4125 /wd4834 /wd4146)

include_directories(../Dependencies/installed/${VCPKG_TARGET_TRIPLET}/include)
include_directories(${CMAKE_BINARY_DIR}/Proto)
include_directories(inc)

# Include sub-projects.
add_subdirectory ("Client")
add_subdirectory ("Proto")
add_subdirectory ("Server")