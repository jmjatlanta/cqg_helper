cmake_minimum_required(VERSION 3.25 )
cmake_policy(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20)

project ( protoBinding )

FILE(GLOB HDRS "./WebAPI/*.h")
FILE(GLOB SRCS "./WebAPI/*.cc")

add_library(protoBinding ${HDRS} ${SRCS} )
TARGET_INCLUDE_DIRECTORIES(protoBinding
    PRIVATE
    ${CMAKE_SOURCE_DIR}/generated
)

TARGET_LINK_LIBRARIES(protoBinding ${PROTOBUF_LIBRARIES})
