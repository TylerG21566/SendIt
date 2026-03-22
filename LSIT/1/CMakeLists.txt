cmake_minimum_required(VERSION 3.24.3)
project(lsit LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect src files
file(GLOB_RECURSE LSIT_SOURCES "src/*.cpp")

add_library(lsit STATIC ${LSIT_SOURCES})
target_include_directories(lsit PUBLIC include/)
target_compile_options(lsit PRIVATE -Wall -Wextra -Wpedantic)

find_package(GTest REQUIRED)

# Collect test files, excluding main.cpp
file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
list(REMOVE_ITEM TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/tests/main.cpp")

add_executable(lsit_tests tests/main.cpp ${TEST_SOURCES})
target_link_libraries(lsit_tests PRIVATE lsit GTest::gtest GTest::gtest_main)
target_compile_options(lsit_tests PRIVATE -fsanitize=address,undefined)
target_link_options(lsit_tests PRIVATE -fsanitize=address,undefined)

enable_testing()
add_test(NAME lsit_tests COMMAND lsit_tests)