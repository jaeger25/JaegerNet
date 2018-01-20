# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.10.0)
project ("JaegerNet")

set(JaegerNet_VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 17)

# cotire
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake")

include(cotire)
include(FindProtobuf)
find_package(Protobuf CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

add_definitions (-DASIO_STANDALONE -DASIO_HAS_STD_SHARED_PTR -DASIO_HAS_STD_ADDRESSOF -DASIO_HAS_STD_ARRAY -DASIO_HAS_STD_TYPE_TRAITS -DASIO_HAS_CSTDINT)

add_compile_options(/W4 /WX /wd4251 /wd4996 /wd4125 /wd4834 /wd4146)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_path(ASIO_INCLUDE_DIR asio.hpp)

include_directories(${ASIO_INCLUDE_DIR})
include_directories(${CMAKE_BINARY_DIR}/Proto)
include_directories(inc)

# Include sub-projects.
add_subdirectory ("Client")
add_subdirectory ("Proto")
add_subdirectory ("Server")