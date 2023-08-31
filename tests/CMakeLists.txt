cmake_minimum_required(VERSION 3.0)

project("gtest")

enable_testing()
add_subdirectory(googletest)

aux_source_directory(./unit TEST_SOURCES)
set(TESTING_FILES 
                ../src/point.cpp
                ../src/line.cpp
                ../src/plane.cpp
                ../src/triangle.cpp
                        )
add_executable(test_app ${TEST_SOURCES}
                        ${TESTING_FILES}
              )

target_include_directories(test_app
                           PRIVATE googletest/
                           PRIVATE ${INCLUDE_DIR}
                           )

target_link_libraries(test_app gtest_main gmock_main)

