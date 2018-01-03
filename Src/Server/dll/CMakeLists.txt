include_directories(../lib)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

include(GenerateExportHeader)

add_library(JaegerNetServer SHARED
    "Module.cpp")

target_link_libraries(JaegerNetServer
    PRIVATE ProtoLib
    PRIVATE ServerLib
    ${PROTOBUF_LIBRARY})

set_property(TARGET JaegerNetServer PROPERTY VERSION ${JaegerNet_VERSION})
set_property(TARGET JaegerNetServer PROPERTY SOVERSION 3)
set_property(TARGET JaegerNetServer PROPERTY INTERFACE_JaegerNetServer_MAJOR_VERSION 3)
set_property(TARGET JaegerNetServer APPEND PROPERTY COMPATIBLE_INTERFACE_STRING JaegerNetServer_MAJOR_VERSION)

generate_export_header(JaegerNetServer
    BASE_NAME JaegerNetServer
    EXPORT_MACRO_NAME JAEGERNET_EXPORT
    EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/JaegerNetServer_Exports.h
    STATIC_DEFINE SHARED_EXPORTS_BUILT_AS_STATIC)

install(TARGETS JaegerNetServer EXPORT JaegerNetServerTargets
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)

install(
  FILES
    ../inc/JaegerNetServer.h
    ../../inc/JaegerNet_Types.h
    ../../inc/JaegerNet_ErrorCodes.h
    "${CMAKE_CURRENT_BINARY_DIR}/JaegerNetServer_Exports.h"
  DESTINATION
    include/JaegerNetServer
  COMPONENT
    Devel
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/JaegerNetServer/JaegerNetServerConfigVersion.cmake"
  VERSION ${JaegerNet_VERSION}
  COMPATIBILITY AnyNewerVersion
)

export(EXPORT JaegerNetServerTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/JaegerNetServer/JaegerNetServerTargets.cmake"
  NAMESPACE JaegerNet::
)
configure_file(CMake/JaegerNetServerConfig.cmake
  "${CMAKE_CURRENT_BINARY_DIR}/JaegerNetServer/JaegerNetServerConfig.cmake"
  COPYONLY
)

set(ConfigPackageLocation share/JaegerNetServer)
install(EXPORT JaegerNetServerTargets
  FILE
    JaegerNetServerTargets.cmake
  NAMESPACE
    JaegerNet::
  DESTINATION
    ${ConfigPackageLocation}
)
install(
  FILES
    cmake/JaegerNetServerConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/JaegerNetServer/JaegerNetServerConfigVersion.cmake"
  DESTINATION
    ${ConfigPackageLocation}
  COMPONENT
    Devel
)

cotire(JaegerNetServer)
