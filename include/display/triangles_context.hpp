#pragma once

#include <vulkan/vulkan.h>

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
  TrianglesContext &operator=(TrianglesContext &&Rhs) = default;
  TrianglesContext(TrianglesContext &&Rhs) = default;
  ~TrianglesContext() = default;

  void run();

private:
  void initVulkan();

  void createWindow();

  void createInstance();
  void createSurface();
  void pickPhysicalDevice();
  void createLogicalDevice();
  void createSwapChain();
  void createImageViews();
  void createGraphicsPipeline();
  void createCommandPool();
  void createCommandBuffer();
  void transition_image_layout(uint32_t imageIndex, vk::ImageLayout oldLayout,
                               vk::ImageLayout newLayout,
                               vk::AccessFlags2 srcAccessMask,
                               vk::AccessFlags2 dstAccessMask,
                               vk::PipelineStageFlags2 srcStageMask,
                               vk::PipelineStageFlags2 dstStageMask);
  void recordCommandBuffer(uint32_t imageIndex);
  void createSyncObjects();
  void drawFrame();

  vk::SurfaceFormatKHR chooseSwapSurfaceFormat(
      const std::vector<vk::SurfaceFormatKHR> &AvailableFormats);
  vk::PresentModeKHR chooseSwapPresentMode(
      const std::vector<vk::PresentModeKHR> &AvailablePresentModes);
  vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR &Capabilities);

  [[nodiscard]] vk::raii::ShaderModule
  createShaderModule(const std::vector<char> &code) const;

  unsigned getQueueFamilyGraphicsIndex(
      const std::vector<vk::QueueFamilyProperties> &QueueFamilyProps) const;

  static bool isDeviceSuitable(const vk::raii::PhysicalDevice &Dev);

  WindowDisplay Window;
  vk::raii::Context Context;
  vk::raii::PhysicalDevice PhysDevice = nullptr;
  vk::raii::Instance Instance = nullptr;
  vk::raii::SurfaceKHR Surface = nullptr;
  vk::raii::Device Device = nullptr;
  vk::raii::Queue GraphicsQueue = nullptr;
  vk::raii::Queue PresentQueue = nullptr;
  vk::raii::PipelineLayout PipelineLayout = nullptr;
  vk::raii::Pipeline GraphicsPipeline = nullptr;
  vk::raii::CommandPool CommandPool = nullptr;
  vk::raii::CommandBuffer CommandBuffer = nullptr;
  vk::raii::Semaphore PresentCompleteSemaphore = nullptr;
  vk::raii::Semaphore RenderFinishedSemaphore = nullptr;
  vk::raii::Fence DrawFence = nullptr;

  vk::raii::SwapchainKHR SwapChain = nullptr;
  std::vector<vk::Image> SwapChainImages;
  vk::SurfaceFormatKHR SwapChainSurfaceFormat;
  vk::Format SwapChainImageFormat = vk::Format::eUndefined;
  std::vector<vk::raii::ImageView> SwapChainImageViews;
  vk::Extent2D SwapChainExtent;

  uint32_t GraphicsFamilyIndex;
  uint32_t PresentFamilyIndex;

  std::vector<const char *> DeviceExtensions = {
      vk::KHRSwapchainExtensionName, vk::KHRSpirv14ExtensionName,
      vk::KHRSynchronization2ExtensionName,
      vk::KHRCreateRenderpass2ExtensionName};
};

} // namespace triangles
