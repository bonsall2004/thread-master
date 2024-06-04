cmake_minimum_required(VERSION 3.20)
project(thread-master)

option(BUILD_EXAMPLES "Enable Examples" OFF)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../binaries)

if(BUILD_EXAMPLES)
  message(STATUS "Examples enabled")
  
  set(SOURCES test.cpp)
  
  add_executable(example ${SOURCES})
  
  target_link_libraries(example PRIVATE thread_master)
  target_include_directories(example PRIVATE ../includes)
else()
  message(STATUS "Examples Disabled")
endif()
