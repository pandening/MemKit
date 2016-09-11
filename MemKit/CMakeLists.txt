cmake_minimum_required(VERSION 3.6)
project(MemKit)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread")

set(SOURCE_FILES main.cpp core/MemKit.h MemKitUtils/MemKitUtils.h
        console/ConsoleRunner.h remote/RemoteRunner.h
        core/RefreshMem.h conf/Configure.h
        RebuildMem/RebuildMemByLogFile.h core/MemKitAutoDumpHandler.h
        MemTimer/TimerFactory.h MemTimer/TemplateOfTimer.h
        core/BigResponseQuery.h MemKitThreadFactory/MemKitThreadFactory.h)
add_executable(MemKit ${SOURCE_FILES})