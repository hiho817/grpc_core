cmake_minimum_required(VERSION 3.8)

project(grpc_core C CXX)
set (CMAKE_EXE_LINKER_FLAGS)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

option(CMAKE_INSTALL_PREFIX "/usr/local")
set(CMAKE_INSTALL_BINDIR ${CMAKE_INSTALL_PREFIX}/bin)
set(CMAKE_INSTALL_LIBDIR ${CMAKE_INSTALL_PREFIX}/lib)
set(CMAKE_INSTALL_INCLUDEDIR ${CMAKE_INSTALL_PREFIX}/include)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/common.cmake)
include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

macro(target_public_headers TARGET)
  set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER "${ARGN}")
endmacro()
 
#### grpc proto file ####
set(PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/protos)
file(GLOB PROTO_FILES "${PROTO_PATH}/*.proto")

foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(FILE_NAME ${PROTO_FILE} NAME_WE)
    list(APPEND PROTONAMES ${FILE_NAME})
endforeach()

foreach(PROTONAME ${PROTONAMES})
    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.pb.cc"
      "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.pb.h"
      "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.grpc.pb.cc" 
      "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.grpc.pb.h"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${CMAKE_CURRENT_LIST_DIR}/protos"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${PROTO_PATH}/${PROTONAME}.proto"
      DEPENDS "${PROTO_PATH}/${PROTONAME}.proto")

    add_library("${PROTONAME}_grpc_proto"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.pb.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.grpc.pb.cc" 
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.grpc.pb.h")
    target_link_libraries("${PROTONAME}_grpc_proto"
      ${_REFLECTION}
      ${_GRPC_GRPCPP}
      ${_PROTOBUF_LIBPROTOBUF})
    
    target_public_headers(
    "${PROTONAME}_grpc_proto" 
    "${PROTO_PATH}/${PROTONAME}.proto" 
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.grpc.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROTONAME}.pb.h")

    INSTALL(TARGETS "${PROTONAME}_grpc_proto"
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endforeach()

#### Self-defined Proto file ####
set(PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/robot_protos)
file(GLOB_RECURSE PROTO_FILES "${PROTO_PATH}/*.proto")

foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(FILE_NAME ${PROTO_FILE} NAME_WE)
    get_filename_component(PROTO_DIR ${PROTO_FILE} DIRECTORY)
    file(RELATIVE_PATH RELATIVE_PROTO_PATH ${CMAKE_CURRENT_LIST_DIR}/robot_protos ${PROTO_DIR})
    # message(STATUS "FOUND PROTO FILES: ${PROTO_FILE}")
    # message(STATUS "FELATIVE_PROTO_PATH: ${RELATIVE_PROTO_PATH}")

    add_custom_command(
      OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.pb.cc"
      "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.pb.h"
      "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.grpc.pb.cc" 
      "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.grpc.pb.h"
      COMMAND ${_PROTOBUF_PROTOC}
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${CMAKE_CURRENT_LIST_DIR}/protos"
        -I "${CMAKE_CURRENT_LIST_DIR}/robot_protos"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${PROTO_FILE}"
      DEPENDS "${PROTO_FILE}")

    add_library("${FILE_NAME}_proto"
    "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.pb.cc"
    "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.grpc.pb.cc" 
    "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.grpc.pb.h")
    target_link_libraries("${FILE_NAME}_proto"
      ${_REFLECTION}
      ${_GRPC_GRPCPP}
      ${_PROTOBUF_LIBPROTOBUF})
    
    target_public_headers(
    "${FILE_NAME}_proto" 
    "${PROTO_FILE}" 
    "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.grpc.pb.h"
    "${CMAKE_CURRENT_BINARY_DIR}/${RELATIVE_PROTO_PATH}/${FILE_NAME}.pb.h")

    INSTALL(TARGETS "${FILE_NAME}_proto"
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )
endforeach()

include_directories("${CMAKE_CURRENT_BINARY_DIR}")

add_subdirectory(src)
