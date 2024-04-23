cmake_minimum_required(VERSION 3.29)

project("io_model" VERSION 0.1 LANGUAGES CXX )

set(EXPORT_COMPILE_COMMANDS ON)

add_library(thread_pool_lib INTERFACE)
target_include_directories(thread_pool_lib INTERFACE thread_pool.hpp)

add_library(business_logic STATIC business_logic.cpp)


add_executable(simple_model simple_model.cpp)
target_link_libraries(simple_model business_logic)

add_executable(multiplexing_and_thread_pool multiplexing_and_thread_pool.cpp)
target_link_libraries(multiplexing_and_thread_pool business_logic)