#include <vulkan/vulkan.h>

#include <algorithm>
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

  vk::InstanceCreateInfo CreateInfo{
      .flags = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR,
      .pApplicationInfo = &AppInfo,
  };

  Instance = vk::raii::Instance(Context, CreateInfo);
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

unsigned findQueueFamilies(const vk::raii::PhysicalDevice &PhysDev) {
  // find the index of the first queue family that supports graphics
  auto QueueFamilyProperties = PhysDev.getQueueFamilyProperties();

  // get the first index into queueFamilyProperties which supports graphics
  auto GraphicsQueueFamilyProperty = std::find_if(
      QueueFamilyProperties.begin(), QueueFamilyProperties.end(),
      [](auto &&QFP) { return QFP.queueFlags & vk::QueueFlagBits::eGraphics; });

  return static_cast<unsigned>(std::distance(QueueFamilyProperties.begin(),
                                             GraphicsQueueFamilyProperty));
}

void TrianglesContext::createLogicalDevice() {
  auto QueueFamilyProperties = PhysDevice.getQueueFamilyProperties();
  auto GraphicQueueIter = std::find_if(
      QueueFamilyProperties.begin(), QueueFamilyProperties.end(),
      [](auto &&QFP) { return QFP.queueFlags & vk::QueueFlagBits::eGraphics; });
  QueueFamilyIndex = static_cast<unsigned>(
      std::distance(QueueFamilyProperties.begin(), GraphicQueueIter));

  vk::DeviceQueueCreateInfo DevQueueCreateInfo{.queueFamilyIndex =
                                                   QueueFamilyIndex,
                                               .queueCount = 1,
                                               .pQueuePriorities = 0};
  vk::StructureChain<vk::PhysicalDeviceFeatures2,
                     vk::PhysicalDeviceVulkan13Features,
                     vk::PhysicalDeviceExtendedDynamicStateFeaturesEXT>
      FeatureChain = {
          {}, {.dynamicRendering = true}, {.extendedDynamicState = true}};
  std::vector<const char *> DeviceExtensions = {
      vk::KHRSwapchainExtensionName, vk::KHRSpirv14ExtensionName,
      vk::KHRSynchronization2ExtensionName,
      vk::KHRCreateRenderpass2ExtensionName};
  vk::DeviceCreateInfo DevCreateInfo{
      .pNext = &FeatureChain.get<vk::PhysicalDeviceFeatures2>(),
      .queueCreateInfoCount = 1,
      .pQueueCreateInfos = &DevQueueCreateInfo,
      .enabledExtensionCount = static_cast<uint32_t>(DeviceExtensions.size()),
      .ppEnabledExtensionNames = DeviceExtensions.data()};
  // Create Device
  Device = vk::raii::Device(PhysDevice, DevCreateInfo);
  GraphicsQueue = vk::raii::Queue(Device, QueueFamilyIndex, 0);
}

} // namespace triangles
