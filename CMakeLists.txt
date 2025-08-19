cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_COMPILER "clang")
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_COLOR_DIAGNOSTICS ON)

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

# Require Vulkan version ≥ 1.3.256 (earliest version when the Vulkan module was available)
if( ${Vulkan_VERSION} VERSION_LESS "1.3.256" )
  message( FATAL_ERROR "Minimum required Vulkan version for C++ modules is 1.3.256. "
           "Found ${Vulkan_VERSION}."
  )
endif()

# set up Vulkan C++ module as a library
add_library( VulkanHppModule )
target_sources( VulkanHppModule PUBLIC 
  FILE_SET CXX_MODULES
  BASE_DIRS ${Vulkan_INCLUDE_DIR}
  FILES ${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
)
target_compile_features( VulkanHppModule PUBLIC cxx_std_20 )
target_link_libraries( VulkanHppModule PUBLIC Vulkan::Headers )
target_compile_definitions( VulkanHppModule PUBLIC
  VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
)

add_executable( vulkan vulkan-triangles.cpp ${DISPLAY} )
# link Vulkan C++ module into your project
target_include_directories( vulkan PRIVATE ${INCLUDE_DIR} )
target_link_libraries( vulkan PRIVATE Vulkan::Vulkan VulkanHppModule glfw glm)

target_compile_definitions( VulkanHppModule PRIVATE
  VULKAN_HPP_DISPATCH_LOADER_DYNAMIC=1
  VULKAN_HPP_NO_STRUCT_CONSTRUCTORS=1
)

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

