cmake_minimum_required(VERSION 3.25 )
cmake_policy(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)

project ( protoBinding )

FILE(GLOB web_HDRS "./WebAPI/*.h")
FILE(GLOB web_SRCS "./WebAPI/*.cc")
FILE(GLOB common_HDRS "./common/*.h")
FILE(GLOB common_SRCS "./common/*.cc")

add_library(protoBinding 
    ${web_HDRS} ${web_SRCS} 
    ${common_HDRS} ${common_SRCS} 
)
TARGET_INCLUDE_DIRECTORIES(protoBinding
    PRIVATE
    ${CMAKE_SOURCE_DIR}/generated
)

TARGET_LINK_LIBRARIES(protoBinding 
    protobuf::libprotobuf
)
