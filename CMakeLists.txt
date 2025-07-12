cmake_minimum_required(VERSION 3.15)

project(triangles)

find_package(Vulkan REQUIRED)
find_package(glfw3 3.3 REQUIRED)
find_package(glm REQUIRED)

set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

aux_source_directory(./src PRIMITIVES)
aux_source_directory(./src/OctTree OCTTREE)
aux_source_directory(./src/intersections INTERSECTIONS)
aux_source_directory(./src/display DISPLAY)

#add_executable(${PROJECT_NAME} ${PRIMITIVES} ${OCTTREE} ${INTERSECTIONS})

add_executable(vulkan vulkan-triangles.cpp ${DISPLAY})

if (VULKAN_FOUND)
  message(STATUS "Found Vulkan,     Including and Linking now")
  target_include_directories(vulkan PRIVATE {Vulkan_INCLUDE_DIRS} ${INCLUDE_DIR})
  target_link_libraries(vulkan PRIVATE Vulkan::Vulkan glfw glm)
endif (VULKAN_FOUND)  

if (false)
target_include_directories( ${PROJECT_NAME}
                            PRIVATE
                            ${INCLUDE_DIR})
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    COMPILE_FLAGS "-Wall -Wextra"
)
endif()

#add_subdirectory(tests)

