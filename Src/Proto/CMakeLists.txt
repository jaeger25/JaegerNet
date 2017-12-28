PROTOBUF_GENERATE_CPP(PROTO_SRC PROTO_HEADER ConnectMessage.proto;ControllerInputMessage.proto;CreateLobbyMessage.proto;JaegerNet.proto)

# Variables needed to propagate through modules
# If more than one layer of cmake use CACHE INTERNAL ...
#set (PROTO_SRCS_VAR ${PROTO_SRCS} CACHE INTERNAL "Sources variable")
set(PROTOBUF_INCLUDE_DIRS ${PROTOBUF_INCLUDE_DIRS} PARENT_SCOPE)
set(PROTOBUF_LIBRARIES ${PROTOBUF_LIBRARIES} PARENT_SCOPE)

set_source_files_properties(${PROTO_SRC} ${PROTO_HEADER} PROPERTIES GENERATED TRUE)

add_custom_target(protobuf_compilation DEPENDS ${PROTO_SRC})

add_library(ProtoLib ${PROTO_SRC} ${PROTO_HDRS})
add_dependencies(ProtoLib protobuf_compilation)

cotire(ProtoLib)
