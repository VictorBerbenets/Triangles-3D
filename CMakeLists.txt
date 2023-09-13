cmake_minimum_required(VERSION 3.15)

project(triangles)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

aux_source_directory(./src PRIMITIVES)
aux_source_directory(./src/OctTree OCTTREE)
aux_source_directory(./src/intersections INTERSECTIONS)

add_executable(${PROJECT_NAME} ${PRIMITIVES} ${OCTTREE} ${INTERSECTIONS})

target_include_directories( ${PROJECT_NAME}
                            PRIVATE
                            ${INCLUDE_DIR})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS " -O3 -Wall -Wextra "
)

add_subdirectory(tests)

