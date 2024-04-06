cmake_minimum_required(VERSION 3.20)
project(thread_master)
set(LIB_NAME thread_master)

set(CMAKE_CXX_STANDARD 20)

add_library(${LIB_NAME}
        includes/Task.h includes/Dispatcher.h
        TaskScheduler.cpp
        includes/TaskScheduler.h)

set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(${LIB_NAME} PUBLIC includes)