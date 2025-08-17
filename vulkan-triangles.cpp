#define GLFW_INCLUDE_VULKAN

#include <iostream>

import vulkan_hpp;
#include <GLFW/glfw3.h>
#if 1

#include "display/triangles_context.hpp"

#include <vulkan/vulkan_hpp_macros.hpp>

#if VULKAN_HPP_DISPATCH_LOADER_DYNAMIC == 1
VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE
#endif
#endif

int main() {
#if 1
  using namespace triangles;

  TrianglesContext App(1000);
  try {
    App.run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
    return EXIT_FAILURE;
  }
#else
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  auto Window = glfwCreateWindow(200, 100, "Vulkan", nullptr, nullptr);
  while (!glfwWindowShouldClose(Window)) {
    glfwPollEvents();
  }
  glfwDestroyWindow(Window);

  glfwTerminate();
#endif
}
