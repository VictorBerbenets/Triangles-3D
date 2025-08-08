#define GLFW_INCLUDE_VULKAN

#include <iostream>

#include <GLFW/glfw3.h>

#include "display/window.hpp"
#include "display/pipeline.hpp"

import vulkan_hpp;

#include <vulkan/vulkan_hpp_macros.hpp>

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main() {
#if ( VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1 )
  // initialize minimal set of function pointers
  VULKAN_HPP_DEFAULT_DISPATCHER.init();
#endif
  using namespace triangles;

  WindowDisplay App;

  try {
    App.display();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }

  VKTrianglesPipeline ("shaders/triangles-shader.vert.spv", "shaders/triangles-shader.frag.spv");
}
