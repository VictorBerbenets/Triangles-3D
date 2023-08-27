cmake_minimum_required(VERSION 3.15)

project(triangles)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SOURCES
        src/main.cpp
        src/point.cpp
        src/line.cpp
        src/plane.cpp
        src/triangle.cpp
        src/diff_intersections.cpp
        src/same_intersections.cpp
        src/interface_intersections.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories( ${PROJECT_NAME}
                            PRIVATE
                            ${INCLUDE_DIR})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS " -Wall -Wextra "
)
