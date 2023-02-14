cmake_minimum_required(VERSION 3.8)
project(poco_http_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # Add the AWS SDK for C++ include directories
find_package(AWSSDK REQUIRED COMPONENTS s3)
include_directories(${AWSSDK_INCLUDE_DIRS})

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(http_server http_server.cc)
target_link_libraries(http_server ${CONAN_LIBS})
target_link_libraries(http_server ${AWSSDK_LIBRARIES})
target_compile_features(http_server PRIVATE cxx_std_17)
