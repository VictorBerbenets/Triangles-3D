#pragma once

#include <string_view>
#include <vector>

#include "display/window.hpp"

import vulkan_hpp;

namespace triangles {

class TrianglesContext {
public:
  TrianglesContext(unsigned W = 800, unsigned H = 600,
                   std::string_view WindowName = "Triangles")
      : Window(W, H, WindowName) {}

  TrianglesContext(const TrianglesContext &Rhs) = delete;
  TrianglesContext &operator=(const TrianglesContext &Rhs) = delete;
  TrianglesContext &operator=(TrianglesContext &&Rhs) = delete;
  TrianglesContext(TrianglesContext &&Rhs) = delete;
  ~TrianglesContext() = default;

  void run();

private:
  void initVulkan();

  void createWindow();

  void cleanup();

  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();

  unsigned getQueueFamilyGraphicsIndex(const std::vector<vk::QueueFamilyProperties> &QueueFamilyProps);

  static bool isDeviceSuitable(const vk::raii::PhysicalDevice &Dev);

  WindowDisplay Window;
  vk::raii::Context Context;
  vk::raii::PhysicalDevice PhysDevice = nullptr;
  vk::raii::Instance Instance = nullptr;
  vk::raii::SurfaceKHR Surface = nullptr;
  vk::raii::Device Device = nullptr;
  vk::raii::Queue GraphicsQueue = nullptr;
  vk::raii::Queue PresentQueue = nullptr;

  const std::vector<const char *> DeviceExtensions = {
      vk::KHRSwapchainExtensionName, vk::KHRSpirv14ExtensionName,
      vk::KHRSynchronization2ExtensionName,
      vk::KHRCreateRenderpass2ExtensionName};
};

} // namespace triangles
