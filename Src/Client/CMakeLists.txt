#include(sdl2)
find_package(sdl2 REQUIRED)

include_directories(inc)

add_subdirectory("dll")
add_subdirectory("exe")
add_subdirectory("lib")
