cmake_minimum_required(VERSION 3.24.3)
project(lsit LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE LSIT_SOURCES "src/*.cpp")
add_library(lsit STATIC ${LSIT_SOURCES})
target_include_directories(lsit PUBLIC include/)
target_compile_options(lsit PRIVATE -Wall -Wextra -Wpedantic)

find_package(GTest CONFIG REQUIRED)
include(GoogleTest)

file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
add_executable(lsit_tests ${TEST_SOURCES})
target_link_libraries(lsit_tests PRIVATE lsit GTest::gtest GTest::gtest_main)
target_compile_options(lsit_tests PRIVATE -fsanitize=address,undefined)
target_link_options(lsit_tests PRIVATE -fsanitize=address,undefined)

enable_testing()                                          # ← must be before discovery
gtest_discover_tests(lsit_tests DISCOVERY_MODE PRE_TEST) # ← defers until test run