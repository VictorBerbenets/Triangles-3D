#include <vulkan/vulkan.h>

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <vector>

import vulkan_hpp;

#include "display/triangles_context.hpp"

namespace triangles {

void TrianglesContext::run() {
  initVulkan();
  while (!glfwWindowShouldClose(Window.Window)) {
    glfwPollEvents();
    drawFrame();
  }
  Device.waitIdle();
}

void TrianglesContext::createWindow() { Window.display(); }

void TrianglesContext::initVulkan() {
  createInstance();
  createSurface();
  pickPhysicalDevice();
  createLogicalDevice();
  createSwapChain();
  createImageViews();
  createGraphicsPipeline();
  createCommandPool();
  createCommandBuffer();
  createSyncObjects();
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

unsigned TrianglesContext::getQueueFamilyGraphicsIndex(
    const std::vector<vk::QueueFamilyProperties> &QueueFamilyProps) const {
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
  GraphicsFamilyIndex = getQueueFamilyGraphicsIndex(QueueFamilyProperties);

  auto QueueFamPropsSize = QueueFamilyProperties.size();
  PresentFamilyIndex =
      PhysDevice.getSurfaceSupportKHR(GraphicsFamilyIndex, *Surface)
          ? GraphicsFamilyIndex
          : QueueFamPropsSize;
  if (PresentFamilyIndex == QueueFamPropsSize) {
    // the graphicsIndex doesn't support present -> look for another family
    // index that supports both graphics and present
    // not std::find_if because we need to keep the iteraiting index i to call
    // getSurfaceSupportKHR
    for (auto i = 0; i < QueueFamPropsSize; i++)
      if ((QueueFamilyProperties[i].queueFlags &
           vk::QueueFlagBits::eGraphics) &&
          PhysDevice.getSurfaceSupportKHR(i, *Surface)) {
        GraphicsFamilyIndex = i;
        PresentFamilyIndex = GraphicsFamilyIndex;
        break;
      }
    if (PresentFamilyIndex == QueueFamPropsSize) {
      // there's nothing like a single family index that supports both graphics
      // and present -> look for another family index that supports present.
      // not std::find_if because we need to keep the iteraiting index i to call
      // getSurfaceSupportKHR
      for (auto i = 0; i < QueueFamPropsSize; i++)
        if (PhysDevice.getSurfaceSupportKHR(i, *Surface)) {
          PresentFamilyIndex = i;
          break;
        }
    }
  }
  if ((GraphicsFamilyIndex == QueueFamPropsSize) ||
      (PresentFamilyIndex == QueueFamPropsSize))
    throw std::runtime_error(
        "Could not find a queue for graphics or present: terminating");

  vk::DeviceQueueCreateInfo DevQueueCreateInfo{.queueFamilyIndex =
                                                   GraphicsFamilyIndex,
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
  GraphicsQueue = vk::raii::Queue(Device, GraphicsFamilyIndex, 0);
  PresentQueue = vk::raii::Queue(Device, PresentFamilyIndex, 0);
}

void TrianglesContext::createSwapChain() {
  auto SurfaceCapabilities = PhysDevice.getSurfaceCapabilitiesKHR(Surface);
  SwapChainSurfaceFormat =
      chooseSwapSurfaceFormat(PhysDevice.getSurfaceFormatsKHR(Surface));
  SwapChainExtent = chooseSwapExtent(SurfaceCapabilities);
  auto minImageCount = std::max(3u, SurfaceCapabilities.minImageCount);
  minImageCount = (SurfaceCapabilities.maxImageCount > 0 &&
                   minImageCount > SurfaceCapabilities.maxImageCount)
                      ? SurfaceCapabilities.maxImageCount
                      : minImageCount;

  uint32_t ImageCount = SurfaceCapabilities.minImageCount + 1;
  if (SurfaceCapabilities.maxImageCount > 0 &&
      ImageCount > SurfaceCapabilities.maxImageCount)
    ImageCount = SurfaceCapabilities.maxImageCount;

  vk::SwapchainCreateInfoKHR SwapChainCreateInfo{
      .flags = vk::SwapchainCreateFlagsKHR(),
      .surface = Surface,
      .minImageCount = minImageCount,
      .imageFormat = SwapChainSurfaceFormat.format,
      .imageColorSpace = SwapChainSurfaceFormat.colorSpace,
      .imageExtent = SwapChainExtent,
      .imageArrayLayers = 1,
      .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
      .preTransform = SurfaceCapabilities.currentTransform,
      .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
      .presentMode =
          chooseSwapPresentMode(PhysDevice.getSurfacePresentModesKHR(Surface)),
      .clipped = vk::True,
      .oldSwapchain = VK_NULL_HANDLE};

  uint32_t queueFamilyIndices[] = {GraphicsFamilyIndex, PresentFamilyIndex};

  if (GraphicsFamilyIndex != PresentFamilyIndex) {
    SwapChainCreateInfo.imageSharingMode = vk::SharingMode::eConcurrent;
    SwapChainCreateInfo.queueFamilyIndexCount = 2;
    SwapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
  } else {
    SwapChainCreateInfo.imageSharingMode = vk::SharingMode::eExclusive;
    SwapChainCreateInfo.queueFamilyIndexCount = 0;
    SwapChainCreateInfo.pQueueFamilyIndices = nullptr;
  }

  SwapChain = vk::raii::SwapchainKHR(Device, SwapChainCreateInfo);
  SwapChainImages = SwapChain.getImages();
  SwapChainImageFormat = SwapChainSurfaceFormat.format;
}

vk::SurfaceFormatKHR TrianglesContext::chooseSwapSurfaceFormat(
    const std::vector<vk::SurfaceFormatKHR> &AvailableFormats) {
  auto PreferedFormatIter =
      std::ranges::find_if(AvailableFormats, [](auto &&AvailFormat) {
        return AvailFormat.format == vk::Format::eB8G8R8A8Srgb &&
               AvailFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
      });
  return PreferedFormatIter != AvailableFormats.end() ? *PreferedFormatIter
                                                      : AvailableFormats[0];
}

vk::PresentModeKHR TrianglesContext::chooseSwapPresentMode(
    const std::vector<vk::PresentModeKHR> &AvailablePresentModes) {
  auto PreferedPresentModeIter =
      std::ranges::find_if(AvailablePresentModes, [](auto &&AvailFormat) {
        return AvailFormat == vk::PresentModeKHR::eMailbox;
      });
  return PreferedPresentModeIter != AvailablePresentModes.end()
             ? *PreferedPresentModeIter
             : vk::PresentModeKHR::eFifo;
}

vk::Extent2D TrianglesContext::chooseSwapExtent(
    const vk::SurfaceCapabilitiesKHR &Capabilities) {
  if (Capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    return Capabilities.currentExtent;
  int Width, Height;
  glfwGetFramebufferSize(Window.Window, &Width, &Height);

  return {std::clamp<uint32_t>(Width, Capabilities.minImageExtent.width,
                               Capabilities.maxImageExtent.width),
          std::clamp<uint32_t>(Height, Capabilities.minImageExtent.height,
                               Capabilities.maxImageExtent.height)};
}

void TrianglesContext::createImageViews() {
  SwapChainImageViews.clear();
  vk::ImageViewCreateInfo ImageViewCreateInfo{
      .viewType = vk::ImageViewType::e2D,
      .format = SwapChainImageFormat,
      .subresourceRange = {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

  for (auto &&Image : SwapChainImages) {
    ImageViewCreateInfo.image = Image;
    SwapChainImageViews.emplace_back(Device, ImageViewCreateInfo);
  }
}

static std::vector<char> readFile(const std::string &filename) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open())
    throw std::runtime_error("failed to open file!");

  std::vector<char> buffer(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(buffer.data(), static_cast<std::streamsize>(buffer.size()));
  file.close();

  return buffer;
}

void TrianglesContext::createGraphicsPipeline() {
  vk::raii::ShaderModule ShaderModule =
      createShaderModule(readFile("slang.spv"));
  vk::PipelineShaderStageCreateInfo VertShaderStageInfo{
      .stage = vk::ShaderStageFlagBits::eVertex,
      .module = ShaderModule,
      .pName = "vertMain"};
  vk::PipelineShaderStageCreateInfo FragShaderStageInfo{
      .stage = vk::ShaderStageFlagBits::eFragment,
      .module = ShaderModule,
      .pName = "fragMain"};
  vk::PipelineShaderStageCreateInfo shaderStages[] = {VertShaderStageInfo,
                                                      FragShaderStageInfo};
  vk::PipelineLayoutCreateInfo PipelineLayoutInfo{.setLayoutCount = 0,
                                                  .pushConstantRangeCount = 0};

  PipelineLayout = vk::raii::PipelineLayout(Device, PipelineLayoutInfo);

  std::vector dynamicStates = {vk::DynamicState::eViewport,
                               vk::DynamicState::eScissor};

  vk::PipelineDynamicStateCreateInfo DynamicState{
      .dynamicStateCount = static_cast<uint32_t>(dynamicStates.size()),
      .pDynamicStates = dynamicStates.data()};

  vk::PipelineVertexInputStateCreateInfo VertexInputInfo;

  vk::PipelineInputAssemblyStateCreateInfo InputAssembly{
      .topology = vk::PrimitiveTopology::eTriangleList};

  vk::PipelineViewportStateCreateInfo ViewportState{.viewportCount = 1,
                                                    .scissorCount = 1};

  vk::PipelineRasterizationStateCreateInfo Rasterizer{
      .depthClampEnable = vk::False,
      .rasterizerDiscardEnable = vk::False,
      .polygonMode = vk::PolygonMode::eFill,
      .cullMode = vk::CullModeFlagBits::eBack,
      .frontFace = vk::FrontFace::eClockwise,
      .depthBiasEnable = vk::False,
      .depthBiasSlopeFactor = 1.0f,
      .lineWidth = 1.0f};

  vk::PipelineMultisampleStateCreateInfo Multisampling{
      .rasterizationSamples = vk::SampleCountFlagBits::e1,
      .sampleShadingEnable = vk::False};

  vk::PipelineColorBlendAttachmentState colorBlendAttachment;
  colorBlendAttachment.colorWriteMask =
      vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
      vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA;
  colorBlendAttachment.blendEnable = vk::False;
  vk::PipelineColorBlendStateCreateInfo ColorBlending{
      .logicOpEnable = vk::False,
      .logicOp = vk::LogicOp::eCopy,
      .attachmentCount = 1,
      .pAttachments = &colorBlendAttachment};

  vk::PipelineRenderingCreateInfo PipelineRenderingCreateInfo{
      .colorAttachmentCount = 1,
      .pColorAttachmentFormats = &SwapChainImageFormat};
  vk::GraphicsPipelineCreateInfo PipelineInfo{
      .pNext = &PipelineRenderingCreateInfo,
      .stageCount = 2,
      .pStages = shaderStages,
      .pVertexInputState = &VertexInputInfo,
      .pInputAssemblyState = &InputAssembly,
      .pViewportState = &ViewportState,
      .pRasterizationState = &Rasterizer,
      .pMultisampleState = &Multisampling,
      .pColorBlendState = &ColorBlending,
      .pDynamicState = &DynamicState,
      .layout = PipelineLayout,
      .renderPass = nullptr};

  GraphicsPipeline = vk::raii::Pipeline(Device, nullptr, PipelineInfo);
}

vk::raii::ShaderModule
TrianglesContext::createShaderModule(const std::vector<char> &Code) const {
  vk::ShaderModuleCreateInfo CreateInfo{
      .codeSize = Code.size() * sizeof(char),
      .pCode = reinterpret_cast<const uint32_t *>(Code.data())};
  return vk::raii::ShaderModule(Device, CreateInfo);
}

void TrianglesContext::createCommandPool() {
  vk::CommandPoolCreateInfo PoolInfo{
      .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
      .queueFamilyIndex = GraphicsFamilyIndex};
  CommandPool = vk::raii::CommandPool(Device, PoolInfo);
}

void TrianglesContext::createCommandBuffer() {
  vk::CommandBufferAllocateInfo AllocInfo{.commandPool = CommandPool,
                                          .level =
                                              vk::CommandBufferLevel::ePrimary,
                                          .commandBufferCount = 1};

  CommandBuffer =
      std::move(vk::raii::CommandBuffers(Device, AllocInfo).front());
}

void TrianglesContext::recordCommandBuffer(uint32_t imageIndex) {
  CommandBuffer.begin({});
  transition_image_layout(
      imageIndex, vk::ImageLayout::eUndefined,
      vk::ImageLayout::eColorAttachmentOptimal,
      {}, // srcAccessMask (no need to wait for previous operations)
      vk::AccessFlagBits2::eColorAttachmentWrite,        // dstAccessMask
      vk::PipelineStageFlagBits2::eTopOfPipe,            // srcStage
      vk::PipelineStageFlagBits2::eColorAttachmentOutput // dstStage
  );
  vk::ClearValue clearColor = vk::ClearColorValue(0.0f, 0.0f, 0.0f, 1.0f);
  vk::RenderingAttachmentInfo attachmentInfo = {
      .imageView = SwapChainImageViews[imageIndex],
      .imageLayout = vk::ImageLayout::eColorAttachmentOptimal,
      .loadOp = vk::AttachmentLoadOp::eClear,
      .storeOp = vk::AttachmentStoreOp::eStore,
      .clearValue = clearColor};
  vk::RenderingInfo renderingInfo = {
      .renderArea = {.offset = {0, 0}, .extent = SwapChainExtent},
      .layerCount = 1,
      .colorAttachmentCount = 1,
      .pColorAttachments = &attachmentInfo};

  CommandBuffer.beginRendering(renderingInfo);
  CommandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics,
                             *GraphicsPipeline);
  CommandBuffer.setViewport(
      0, vk::Viewport(0.0f, 0.0f, static_cast<float>(SwapChainExtent.width),
                      static_cast<float>(SwapChainExtent.height), 0.0f, 1.0f));
  CommandBuffer.setScissor(0, vk::Rect2D(vk::Offset2D(0, 0), SwapChainExtent));
  CommandBuffer.draw(3, 1, 0, 0);
  CommandBuffer.endRendering();
  transition_image_layout(imageIndex, vk::ImageLayout::eColorAttachmentOptimal,
                          vk::ImageLayout::ePresentSrcKHR,
                          vk::AccessFlagBits2::eColorAttachmentWrite, {},
                          vk::PipelineStageFlagBits2::eColorAttachmentOutput,
                          vk::PipelineStageFlagBits2::eBottomOfPipe);
  CommandBuffer.end();
}

void TrianglesContext::transition_image_layout(
    uint32_t imageIndex, vk::ImageLayout oldLayout, vk::ImageLayout newLayout,
    vk::AccessFlags2 srcAccessMask, vk::AccessFlags2 dstAccessMask,
    vk::PipelineStageFlags2 srcStageMask,
    vk::PipelineStageFlags2 dstStageMask) {
  vk::ImageMemoryBarrier2 barrier = {
      .srcStageMask = srcStageMask,
      .srcAccessMask = srcAccessMask,
      .dstStageMask = dstStageMask,
      .dstAccessMask = dstAccessMask,
      .oldLayout = oldLayout,
      .newLayout = newLayout,
      .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
      .image = SwapChainImages[imageIndex],
      .subresourceRange = {.aspectMask = vk::ImageAspectFlagBits::eColor,
                           .baseMipLevel = 0,
                           .levelCount = 1,
                           .baseArrayLayer = 0,
                           .layerCount = 1}};
  vk::DependencyInfo dependencyInfo = {.dependencyFlags = {},
                                       .imageMemoryBarrierCount = 1,
                                       .pImageMemoryBarriers = &barrier};
  CommandBuffer.pipelineBarrier2(dependencyInfo);
}

void TrianglesContext::createSyncObjects() {
  PresentCompleteSemaphore =
      vk::raii::Semaphore(Device, vk::SemaphoreCreateInfo());
  RenderFinishedSemaphore =
      vk::raii::Semaphore(Device, vk::SemaphoreCreateInfo());
  DrawFence =
      vk::raii::Fence(Device, {.flags = vk::FenceCreateFlagBits::eSignaled});
}

void TrianglesContext::drawFrame() {
  auto [result, imageIndex] = SwapChain.acquireNextImage(
      UINT64_MAX, *PresentCompleteSemaphore, nullptr);
  recordCommandBuffer(imageIndex);
  Device.resetFences(*DrawFence);
  vk::PipelineStageFlags waitDestinationStageMask(
      vk::PipelineStageFlagBits::eColorAttachmentOutput);
  const vk::SubmitInfo SubmitInfo =
      vk::SubmitInfo()
          .setWaitSemaphores(*PresentCompleteSemaphore)
          .setWaitDstStageMask(waitDestinationStageMask)
          .setCommandBuffers(*CommandBuffer)
          .setSignalSemaphores(*RenderFinishedSemaphore);
  if (!*GraphicsQueue)
    throw std::runtime_error("Failed to get graphics queue");

  GraphicsQueue.submit(SubmitInfo, *DrawFence);
  while (vk::Result::eTimeout ==
         Device.waitForFences(*DrawFence, vk::True, UINT64_MAX))
    ;

  const vk::PresentInfoKHR presentInfoKHR =
      vk::PresentInfoKHR()
          .setWaitSemaphores(*RenderFinishedSemaphore)
          .setSwapchains(*SwapChain)
          .setImageIndices(imageIndex)
          .setPResults(nullptr);
  result = PresentQueue.presentKHR(presentInfoKHR);
}

} // namespace triangles
