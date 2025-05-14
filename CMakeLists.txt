cmake_minimum_required(VERSION 3.5)

project(mcu_async VERSION 0.2.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${SRC_DIR})

file(GLOB_RECURSE LIB_SOURCES
    "${SRC_DIR}/*.cpp"
    "${SRC_DIR}/*.h"
)

add_library(mcuasync STATIC ${LIB_SOURCES})

target_compile_options(mcuasync PRIVATE -Wall -Wextra)
target_include_directories(mcuasync PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(test test/test_main.cpp)
target_link_libraries(test mcuasync)
target_include_directories(test PRIVATE ${SRC_DIR})

add_executable(example examples/example_main.cpp)
target_link_libraries(example mcuasync)
target_include_directories(example PRIVATE ${SRC_DIR})

