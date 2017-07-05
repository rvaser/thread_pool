cmake_minimum_required(VERSION 3.2)
project(thread_pool)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11")

option(thread_pool_build_tests "Build thread_pool google tests" OFF)

add_library(thread_pool STATIC
    src/thread_pool.cpp)

target_include_directories(thread_pool PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

if (thread_pool_build_tests)
    file(COPY test/data DESTINATION ${PROJECT_BINARY_DIR})
    set(thread_pool_test_data_path ${PROJECT_BINARY_DIR}/data/)
    configure_file(
        "${PROJECT_SOURCE_DIR}/test/thread_pool_test_config.h.in"
        "${PROJECT_BINARY_DIR}/config/thread_pool_test_config.h"
    )
    include_directories(${PROJECT_BINARY_DIR}/config)

    add_executable(thread_pool_test test/thread_pool_test.cpp)

    add_subdirectory(vendor/googletest/googletest EXCLUDE_FROM_ALL)
    target_link_libraries(thread_pool_test thread_pool gtest_main)
endif(thread_pool_build_tests)

install(TARGETS thread_pool DESTINATION lib)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/thread_pool DESTINATION include)
