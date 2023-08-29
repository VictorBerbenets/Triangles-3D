cmake_minimum_required(VERSION 3.0)

project("gtest")

enable_testing()
add_subdirectory(googletest)

set(SOURCES unit/main.cpp
            unit/linear_eq_solver.cpp)

add_executable(test_app ${SOURCES})

message("incl dir: ${INCLUDE_DIR}")

target_include_directories(test_app
                           PRIVATE googletest/
                           PRIVATE ${INCLUDE_DIR}
                           )

target_link_libraries(test_app gtest_main gmock_main)

