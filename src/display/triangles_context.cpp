#include <vulkan/vulkan.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <vector>

import vulkan_hpp;

#include "display/triangles_context.hpp"

namespace triangles {

void TrianglesContext::run() {
  initVulkan();
  createWindow();
  cleanup();
}

void TrianglesContext::createWindow() { Window.display(); }

void TrianglesContext::cleanup() {}

void TrianglesContext::initVulkan() {
  createInstance();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
}

void TrianglesContext::createInstance() {
  constexpr vk::ApplicationInfo AppInfo{
      .pApplicationName = "Triangles",
      .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
      .pEngineName = "No Engine",
      .engineVersion = VK_MAKE_VERSION(1, 0, 0),
      .apiVersion = vk::ApiVersion14};

  uint32_t glfwExtensionCount = 0;
  auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

  // Check if the required GLFW extensions are supported by the Vulkan
  // implementation.
  auto extensionProperties = Context.enumerateInstanceExtensionProperties();
  for (uint32_t i = 0; i < glfwExtensionCount; ++i) {
    if (std::ranges::none_of(
            extensionProperties,
            [glfwExtension = glfwExtensions[i]](auto const &extensionProperty) {
              return std::strcmp(extensionProperty.extensionName,
                                 glfwExtension) == 0;
            }))
      throw std::runtime_error("Required GLFW extension not supported: " +
                               std::string(glfwExtensions[i]));
  }

  vk::InstanceCreateInfo CreateInfo{
      .flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
      .pApplicationInfo = &AppInfo,
      .enabledExtensionCount = glfwExtensionCount,
      .ppEnabledExtensionNames = glfwExtensions};

  Instance = vk::raii::Instance(Context, CreateInfo);
}

void TrianglesContext::createSurface() {
  VkSurfaceKHR TmpSurf;
  if (auto I = glfwCreateWindowSurface(*Instance, Window.Window, nullptr,
                                       &TmpSurf) != VK_SUCCESS)
    throw std::runtime_error(std::format(
        "failed to create window surface! Error code: {}", std::to_string(I)));

  Surface = vk::raii::SurfaceKHR(Instance, TmpSurf);
}

void TrianglesContext::pickPhysicalDevice() {
  auto Devices = Instance.enumeratePhysicalDevices() |
                 std::views::filter(isDeviceSuitable);
  if (Devices.empty())
    throw std::runtime_error("failed to find GPUs with Vulkan support!");

  PhysDevice = vk::raii::PhysicalDevice(Devices.front());
}

bool TrianglesContext::isDeviceSuitable(const vk::raii::PhysicalDevice &Dev) {
  auto DevProperties = Dev.getProperties();
  auto DevFeatures = Dev.getFeatures();
  return DevProperties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu &&
         DevFeatures.geometryShader;
}

unsigned TrianglesContext::getQueueFamilyGraphicsIndex(const std::vector<vk::QueueFamilyProperties> &QueueFamilyProps) {
  // find the index of the first queue family that supports graphics
  auto QueueFamilyProperties = PhysDevice.getQueueFamilyProperties();

  // get the first index into queueFamilyProperties which supports graphics
  auto GraphicsQueueFamilyProperty = std::find_if(
      QueueFamilyProperties.begin(), QueueFamilyProperties.end(),
      [](auto &&QFP) { return QFP.queueFlags & vk::QueueFlagBits::eGraphics; });

  return std::distance(QueueFamilyProperties.begin(),
                                             GraphicsQueueFamilyProperty);
}

void TrianglesContext::createLogicalDevice() {
  auto QueueFamilyProperties = PhysDevice.getQueueFamilyProperties();
  auto GraphicsIndex = getQueueFamilyGraphicsIndex(QueueFamilyProperties);

  auto QueueFamPropsSize = QueueFamilyProperties.size();
  auto PresentIndex = PhysDevice.getSurfaceSupportKHR(GraphicsIndex, *Surface)
                          ? GraphicsIndex
                          : QueueFamPropsSize;
  if (PresentIndex == QueueFamPropsSize) {
    // the graphicsIndex doesn't support present -> look for another family
    // index that supports both graphics and present
    // not std::find_if because we need to keep the iteraiting index i to call
    // getSurfaceSupportKHR
    for (auto i = 0; i < QueueFamPropsSize; i++)
      if ((QueueFamilyProperties[i].queueFlags &
           vk::QueueFlagBits::eGraphics) &&
          PhysDevice.getSurfaceSupportKHR(i, *Surface)) {
        GraphicsIndex = i;
        PresentIndex = GraphicsIndex;
        break;
      }
    if (PresentIndex == QueueFamPropsSize) {
      // there's nothing like a single family index that supports both graphics
      // and present -> look for another family index that supports present.
      // not std::find_if because we need to keep the iteraiting index i to call
      // getSurfaceSupportKHR
      for (auto i = 0; i < QueueFamPropsSize; i++)
        if (PhysDevice.getSurfaceSupportKHR(i, *Surface)) {
          PresentIndex = i;
          break;
        }
    }
  }
  if ((GraphicsIndex == QueueFamPropsSize) ||
      (PresentIndex == QueueFamPropsSize))
    throw std::runtime_error(
        "Could not find a queue for graphics or present: terminating");

  vk::DeviceQueueCreateInfo DevQueueCreateInfo{.queueFamilyIndex =
                                                   GraphicsIndex,
                                               .queueCount = 1,
                                               .pQueuePriorities = 0};

  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      FeatureChain = {
          {}, {.dynamicRendering = true}, {.extendedDynamicState = true}};

  vk::DeviceCreateInfo DevCreateInfo{
      .pNext = &FeatureChain.get<vk::PhysicalDeviceFeatures2>(),
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &DevQueueCreateInfo,
      .enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size()),
      .ppEnabledExtensionNames = DeviceExtensions.data()};
  // Create Device
  Device = vk::raii::Device(PhysDevice, DevCreateInfo);
  GraphicsQueue = vk::raii::Queue(Device, GraphicsIndex, 0);
  PresentQueue = vk::raii::Queue(Device, PresentIndex, 0);
}

} // namespace triangles
