cmake_minimum_required(VERSION 3.15)

project(triangles)

add_executable(main src/main.cpp)

target_include_directories( main
                            PRIVATE
                            include/)
set_target_properties(
    main PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS " -Wall -Wextra "
)
