cmake_minimum_required(VERSION 3.17)
project(JSCore)

set(CMAKE_CXX_STANDARD 20)

aux_source_directory(${PROJECT_SOURCE_DIR}/src SRC_LIST)
add_executable(JSCore ${SRC_LIST})

target_link_libraries(JSCore
        "-framework Foundation"
        "-framework CoreFoundation"
        "-framework JavaScriptCore")