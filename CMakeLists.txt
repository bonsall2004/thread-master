cmake_minimum_required(VERSION 3.20)
project(thread_master)
set(LIB_NAME thread_master)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../../binaries)

add_library(${LIB_NAME}
            includes/Task.hpp includes/Dispatcher.hpp
            src/TaskScheduler.cpp
            includes/TaskScheduler.hpp
            )

add_subdirectory(examples)

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${LIB_NAME} PUBLIC includes)