cmake_minimum_required(VERSION 3.6)
project(MemKit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")

set(SOURCE_FILES main.cpp MemKit.h MemKitUtils.h ConsoleRunner.h RemoteRunner.h RefreshMem.h)
add_executable(MemKit ${SOURCE_FILES})