cmake_minimum_required(VERSION 3.5)

project(lib_mcu_async VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

include_directories(${SRC_DIR})

file(GLOB_RECURSE LIB_SOURCES 
    "${SRC_DIR}/*.cpp"
)

add_library(mcu_async STATIC ${LIB_SOURCES})

target_compile_options(mcu_async PRIVATE -Wall -Wextra)
target_include_directories(mcu_async PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

add_executable(mcu_async_test test/test_main.cpp)
target_link_libraries(mcu_async_test mcu_async)
target_include_directories(mcu_async_test PRIVATE ${SRC_DIR})

add_executable(mcu_async_example examples/example_main.cpp)
target_link_libraries(mcu_async_example mcu_async)
target_include_directories(mcu_async_example PRIVATE ${SRC_DIR})

