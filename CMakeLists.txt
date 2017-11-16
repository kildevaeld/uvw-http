cmake_minimum_required(VERSION 3.9)
project(uvw_http)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTING "Enable testing with ctest." OFF)
add_subdirectory(vendor/uvw)
include_directories(vendor/http-parser)

set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(include)
include_directories(vendor/uvw/src)
add_subdirectory(src)


add_subdirectory(example)