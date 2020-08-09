cmake_minimum_required(VERSION 3.17)
project(JSCore)

set(CMAKE_CXX_STANDARD 20)

add_executable(JSCore main.cpp)

target_link_libraries(JSCore
        "-framework Foundation"
        "-framework CoreFoundation"
        "-framework JavaScriptCore")