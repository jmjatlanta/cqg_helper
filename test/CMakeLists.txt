cmake_minimum_required(VERSION 3.25 )
cmake_policy(VERSION 3.25)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

project ( cqg_helper_tests )

file(GLOB cqg_helper "../src/*.cpp" )
file(GLOB util_src "../src/util/*.cpp" )
file(GLOB proto_common "../WebAPI/common/*.cpp")
file(GLOB proto_WebAPI "../WebAPI/WebAPI/*.cpp")

find_package(Threads REQUIRED)
find_package(Boost 1.69 COMPONENTS log log_setup REQUIRED)

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable( cqg_helper_tests 
    mock_server.cpp
    mock_server_test.cpp
    logon.cpp
    ${cqg_helper} ${proto_WebAPI} ${proto_common} )

target_include_directories(cqg_helper_tests PRIVATE 
    ../src
    ../generated
    ../websocketpp
)

target_link_libraries(cqg_helper_tests 
    protoBinding
    protobuf::libprotobuf
    Threads::Threads
    pthread
    GTest::gtest
    GTest::gtest_main
    Boost::log
    Boost::log_setup
    OpenSSL::SSL
    OpenSSL::Crypto
)

add_executable( cqg_mock_server mock_server_main.cpp mock_server.cpp ${cqg_helper} ${proto_WebAPI} ${proto_common} )
target_include_directories(cqg_mock_server PRIVATE
    ../src
    ../generated
    ../websocketpp
)
target_link_libraries(cqg_mock_server
    protoBinding
    protobuf::libprotobuf
    Threads::Threads
    pthread
    Boost::log
    Boost::log_setup
    OpenSSL::SSL
    OpenSSL::Crypto
)

