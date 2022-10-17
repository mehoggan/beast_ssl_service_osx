cmake_minimum_required(VERSION 3.0.2)
project(dagtasks)

option(BUILD_SHARED_LIBS "Build shared libraries over static library." ON)
option(CLONE_EXTERNALS_GIT "Use git protocol while clone external deps." OFF)

if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  set(CMAKE_MACOSX_RPATH 1)
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING
    "If CMAKE_BUILD_TYPE is not set, set it to debug globally." FORCE)
endif()
message(STATUS "Going to configure in \"${CMAKE_BUILD_TYPE}\" mode...")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/c++17.cmake)

# External Library (openssl)
set(OPENSSL_ROOT "${CMAKE_BINARY_DIR}/openssl" CACHE STRING
  "Root directory for where openssl build system installs itself." FORCE)
file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/openssl")
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.openssl.txt.in
  ${CMAKE_BINARY_DIR}/openssl-download/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/openssl.cmake)
link_directories("${CMAKE_CURRENT_BINARY_DIR}/openssl/lib")
set(OPENSSL_LIB "ssl.3" CACHE STRING
  "The name of the library to use for openssl linking." FORCE)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/openssl/include")
set(ENV{OPENSSL_ROOT_DIR} "${CMAKE_CURRENT_BINARY_DIR}/openssl")
message(STATUS "OPENSSL_ROOT_DIR = $ENV{OPENSSL_ROOT_DIR} before configure.")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/openssl/include")

# External Library (boost)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.boostbeast.txt.in
  ${CMAKE_BINARY_DIR}/boostbeast-download/CMakeLists.txt)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/boostbeast.cmake)
set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_BINARY_DIR}/boostbeast/include"
  CACHE INTERNAL "Boost_INCLUDE_DIR")
include_directories("${CMAKE_BINARY_DIR}/boostbeast/include")
link_directories("${CMAKE_BINARY_DIR}/boostbeast/lib")

add_subdirectory(src)
