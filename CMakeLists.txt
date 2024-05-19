cmake_minimum_required(VERSION 3.28)
project(ProcessScheduling)

set(CMAKE_CXX_STANDARD 17)
include_directories(${PROJECT_SOURCE_DIR}/class)

add_executable(ProcessScheduling main.cpp
        class/Scheduler.h
        class/generateRandomProcess.h)
