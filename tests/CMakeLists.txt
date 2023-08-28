cmake_minimum_required(VERSION 3.0)

project("gtest")

enable_testing()
add_subdirectory(googletest)

add_executable(test_app src/main.cpp)
target_include_directories(test_app
                           PRIVATE
                           googletest/
                           )

target_link_libraries(test_app gtest_main gmock_main)