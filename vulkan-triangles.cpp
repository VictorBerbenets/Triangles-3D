#define GLFW_INCLUDE_VULKAN

#include <iostream>

#include <GLFW/glfw3.h>

import vulkan_hpp;

#include "display/triangles_context.hpp"

#include <vulkan/vulkan_hpp_macros.hpp>

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif

int main() {
  using namespace triangles;

  TrianglesContext App(1000);
  try {
    App.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
}
