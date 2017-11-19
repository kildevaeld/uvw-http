cmake_minimum_required(VERSION 3.9)
project(uvw_http)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


#option(BUILD_TESTING "Enable testing with ctest." OFF)
#add_subdirectory(vendor/uvw)



set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})


option(UVEXT_SYSTEM_SSL "Use system ssl" off)
option(UVEXT_EXAMPLE "Build example" off)
option(UVEXT_SYSTEM_UV "Use system libuv" off)

#include(cmake/http-parser.cmake)

if (NOT ${UVEXT_SYSTEM_UV})
	message(STATUS "Using local libuv")
	include(cmake/uv.cmake)
else()
	message(STATUS "Using system libuv")
endif (NOT ${UVEXT_SYSTEM_UV})



if (NOT ${UVEXT_SYSTEM_SSL})
	message(STATUS "Using local boringssl")
	#add_subdirectory(libs/boringssl)
	#include_directories(libs/boringssl/include)
else()
	message(STATUS "Using system openssl")
	if (APPLE)
		include_directories(/usr/local/opt/openssl/lib)
		link_directories(/usr/local/opt/openssl/include)
	endif(APPLE)


endif(NOT ${UVEXT_SYSTEM_SSL})



include_directories(vendor/http-parser)


include_directories(include)
include_directories(vendor/uvw/src)
add_subdirectory(src)


add_subdirectory(example)