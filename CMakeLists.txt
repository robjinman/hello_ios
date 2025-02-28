cmake_minimum_required(VERSION 3.15)
project(HelloIOS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(tinyxml2 REQUIRED)

set(MACOSX_BUNDLE_BUNDLE_NAME "nova")
set(MACOSX_BUNDLE_BUNDLE_VERSION "0.1")
set(MACOSX_BUNDLE_GUI_IDENTIFIER "io.deadbeef.nova")
set(MACOSX_BUNDLE_LONG_VERSION_STRING "0.1")
set(MACOSX_BUNDLE_SHORT_VERSION_STRING "0.1")

add_executable(hello MACOSX_BUNDLE src/hello.cpp src/hello.mm)

target_link_libraries(hello
  PRIVATE
    tinyxml2::tinyxml2
    "-framework UIKit"
)
