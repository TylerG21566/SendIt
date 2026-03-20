cmake_minimum_required(VERSION 3.24.3) # declare min CMAKE version
# declare project
project(lsit LANGUAGES CXX)
# set the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# add my library
add_library(lsit INTERFACE)
# tell cmake where your headers are
target_include_directories(lsit INTERFACE include/)
# enable compiler warnings
target_compile_options(lsit INTERFACE -Wall -Wextra -Wpedantic)
# find and link GoogleTest
find_package(GTest REQUIRED)
# Add test executable
add_executable(lsit_tests tests/main.cpp)
# link your test executable
target_link_libraries(lsit_tests PRIVATE lsit GTest::gtest GTest::gtest_main)
# enabling Asan
target_compile_options(lsit_tests PRIVATE -fsanitize=address,undefined)
# enabling UBSan
target_link_options(lsit_tests PRIVATE -fsanitize=address,undefined)
# enable CTest
enable_testing()
add_test(NAME lsit_tests COMMAND lsit_tests)