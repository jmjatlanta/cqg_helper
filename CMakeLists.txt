cmake_minimum_required(VERSION 3.25 )
cmake_policy(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project ( cqg )

find_package(OpenSSL REQUIRED)
# generate protobuf files
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# build the .cc files from the .proto files in WebAPI and common
FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/WebAPI PROTOMODEL_PATH)
FILE(TO_NATIVE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/generated PROTOBINDING_PATH)

FILE(GLOB DATAMODEL_PROTOS "${CMAKE_CURRENT_SOURCE_DIR}/WebAPI/WebAPI/*.proto")

FOREACH(proto ${DATAMODEL_PROTOS})
    FILE(TO_NATIVE_PATH ${proto} proto_native)
    EXECUTE_PROCESS(COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
        --proto_path=${PROTOMODEL_PATH} 
        --cpp_out=${PROTOBINDING_PATH} 
        ${proto_native}
        RESULT_VARIABLE rv)
    IF(${rv})
        MESSAGE("Generation of data model returned ${rv} for proto ${proto_native}")
    ENDIF()
ENDFOREACH(proto)

FILE(GLOB DATAMODEL_COMMON "${CMAKE_CURRENT_SOURCE_DIR}/WebAPI/common/*.proto")

FOREACH(proto ${DATAMODEL_COMMON})
    FILE(TO_NATIVE_PATH ${proto} proto_native)
    EXECUTE_PROCESS(COMMAND ${Protobuf_PROTOC_EXECUTABLE} 
        --proto_path=${PROTOMODEL_PATH} 
        --cpp_out=${PROTOBINDING_PATH} 
        ${proto_native}
        RESULT_VARIABLE rv)
    IF(${rv})
        MESSAGE("Generation of data model returned ${rv} for proto ${proto_native}")
    ENDIF()
ENDFOREACH(proto)

add_compile_definitions(USE_SSL)

add_subdirectory( generated )
add_subdirectory( websocketpp )
add_subdirectory ( src )
add_subdirectory ( test )

