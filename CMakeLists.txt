cmake_minimum_required(VERSION 3.15)
project(HelloIOS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/ios-cmake/ios.toolchain.cmake)
set(PLATFORM "OS64")

add_executable(hello src/main.cpp)
