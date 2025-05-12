#include "framework.h"
#include "vk_device.h"
#include "lowland/landen/vulkan/vk_container.h"
// std headers
#include <cstring>
#include <iostream>
#include <set>
#include <unordered_set>


namespace vkc
{

   // local callback functions
   static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
      VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
      VkDebugUtilsMessageTypeFlagsEXT messageType,
      const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
      void* pUserData) {
      std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

      return VK_FALSE;
   }

   VkResult CreateDebugUtilsMessengerEXT(
      VkInstance instance,
      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
      const VkAllocationCallbacks* pAllocator,
      VkDebugUtilsMessengerEXT* pDebugMessenger)
   {
      auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
         instance,
         "vkCreateDebugUtilsMessengerEXT");
      if (func != nullptr) {
         return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
      }
      else {
         return VK_ERROR_EXTENSION_NOT_PRESENT;
      }
   }

   void DestroyDebugUtilsMessengerEXT(
      VkInstance instance,
      VkDebugUtilsMessengerEXT debugMessenger,
      const VkAllocationCallbacks* pAllocator) {
      auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
         instance,
         "vkDestroyDebugUtilsMessengerEXT");
      if (func != nullptr) {
         func(instance, debugMessenger, pAllocator);
      }
   }

   // class member functions
   //VkcDevice::VkcDevice(vks::VulkanDevice* pdevice) : m_vkdevice{pdevice->logicalDevice} {
   VkcDevice::VkcDevice()
   {

      surface_ = nullptr;
      presentQueue_ = nullptr;
      graphicsQueue_ = nullptr;

      createInstance();
      setupDebugMessenger();
      createSurface();
      pickPhysicalDevice();
      createLogicalDevice();
      createCommandPool();
   }

   VkcDevice::~VkcDevice()
   {
      vkDestroyCommandPool(m_vkdevice, commandPool, nullptr);
      vkDestroyDevice(m_vkdevice, nullptr);

      if (enableValidationLayers) {
         DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
      }

      if (surface_)
      {
         vkDestroySurfaceKHR(instance, surface_, nullptr);
      }
      vkDestroyInstance(instance, nullptr);

   }


   void VkcDevice::initialize_device(vkc::VkContainer* pvkcontainer)
   {

      m_pvkcontainer = pvkcontainer;

   }


   void VkcDevice::createInstance()
   {

      if (enableValidationLayers && !checkValidationLayerSupport()) {
         throw std::runtime_error("validation layers requested, but not available!");
      }

      VkApplicationInfo appInfo = {};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = "LittleVulkanEngine App";
      appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.pEngineName = "No Engine";
      appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
      appInfo.apiVersion = VK_API_VERSION_1_0;

      VkInstanceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      createInfo.pApplicationInfo = &appInfo;

      auto extensions = getRequiredExtensions();
      createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
      createInfo.ppEnabledExtensionNames = extensions.data();

      VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
      if (enableValidationLayers) {
         createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
         createInfo.ppEnabledLayerNames = validationLayers.data();

         populateDebugMessengerCreateInfo(debugCreateInfo);
         createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
      }
      else {
         createInfo.enabledLayerCount = 0;
         createInfo.pNext = nullptr;
      }

      if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
         throw std::runtime_error("failed to create instance!");
      }

      hasGflwRequiredInstanceExtensions();
   }


   void VkcDevice::pickPhysicalDevice()
   {
      uint32_t deviceCount = 0;
      vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
      if (deviceCount == 0) {
         throw std::runtime_error("failed to find GPUs with Vulkan support!");
      }
      std::cout << "Device count: " << deviceCount << std::endl;
      std::vector<VkPhysicalDevice> devices(deviceCount);
      vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

      for (const auto& pvkcdevice : devices) {
         if (isDeviceSuitable(pvkcdevice)) {
            physicalDevice = pvkcdevice;
            break;
         }
      }

      if (physicalDevice == VK_NULL_HANDLE) {
         throw std::runtime_error("failed to find a suitable GPU!");
      }

      vkGetPhysicalDeviceProperties(physicalDevice, &properties);
      std::cout << "physical pvkcdevice: " << properties.deviceName << std::endl;
   }


   void VkcDevice::createLogicalDevice()
   {

      QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

      std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
      std::set<uint32_t> uniqueQueueFamilies;
      if (indices.graphicsFamilyHasValue)
      {
         uniqueQueueFamilies.insert(indices.graphicsFamily);
      }
      if (indices.presentFamilyHasValue)
      {
         uniqueQueueFamilies.insert(indices.presentFamily);
      }

      float queuePriority = 1.0f;
      for (uint32_t queueFamily : uniqueQueueFamilies)
      {
         VkDeviceQueueCreateInfo queueCreateInfo = {};
         queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
         queueCreateInfo.queueFamilyIndex = queueFamily;
         queueCreateInfo.queueCount = 1;
         queueCreateInfo.pQueuePriorities = &queuePriority;
         queueCreateInfos.push_back(queueCreateInfo);
      }

      VkPhysicalDeviceFeatures deviceFeatures = {};
      deviceFeatures.samplerAnisotropy = VK_TRUE;

      VkDeviceCreateInfo createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

      createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
      createInfo.pQueueCreateInfos = queueCreateInfos.data();

      createInfo.pEnabledFeatures = &deviceFeatures;
      createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
      createInfo.ppEnabledExtensionNames = deviceExtensions.data();

      // might not really be necessary anymore because pvkcdevice specific validation layers
      // have been deprecated
      if (enableValidationLayers)
      {
         createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
         createInfo.ppEnabledLayerNames = validationLayers.data();
      }
      else
      {
         createInfo.enabledLayerCount = 0;
      }

      if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &m_vkdevice) != VK_SUCCESS)
      {
         throw std::runtime_error("failed to create logical pvkcdevice!");
      }
      if (indices.graphicsFamilyHasValue)
      {
         vkGetDeviceQueue(m_vkdevice, indices.graphicsFamily, 0, &graphicsQueue_);
      }
      if (indices.presentFamilyHasValue)
      {
         vkGetDeviceQueue(m_vkdevice, indices.presentFamily, 0, &presentQueue_);
      }
   }


   void VkcDevice::createCommandPool()
   {
      QueueFamilyIndices queueFamilyIndices = findPhysicalQueueFamilies();

      VkCommandPoolCreateInfo poolInfo = {};
      poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
      poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily;
      poolInfo.flags =
         VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

      if (vkCreateCommandPool(m_vkdevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
         throw std::runtime_error("failed to create command pool!");
      }
   }

   //void VkcDevice::createSurface() { window.createWindowSurface(instance, &surface_); }

   void VkcDevice::createSurface() {}


   bool VkcDevice::isDeviceSuitable(VkPhysicalDevice pvkcdevice)
   {

      QueueFamilyIndices indices = findQueueFamilies(pvkcdevice);

      bool extensionsSupported = checkDeviceExtensionSupport(pvkcdevice);

      bool swapChainAdequate = false;
      if (surface_)
      {
         if (extensionsSupported)
         {
            SwapChainSupportDetails swapChainSupport = querySwapChainSupport(pvkcdevice);
            swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
         }
      }
      else
      {

         swapChainAdequate = true;

      }

      VkPhysicalDeviceFeatures supportedFeatures;
      vkGetPhysicalDeviceFeatures(pvkcdevice, &supportedFeatures);

      return (!surface_ || indices.isComplete()) && extensionsSupported && swapChainAdequate &&
         supportedFeatures.samplerAnisotropy;

   }


   void VkcDevice::populateDebugMessengerCreateInfo(
      VkDebugUtilsMessengerCreateInfoEXT& createInfo)
   {

      createInfo = {};
      createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
      createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
         VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
         VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
      createInfo.pfnUserCallback = debugCallback;
      createInfo.pUserData = nullptr;  // Optional

   }


   void VkcDevice::setupDebugMessenger()
   {

      if (!enableValidationLayers)
      {

         return;

      }

      VkDebugUtilsMessengerCreateInfoEXT createInfo;

      populateDebugMessengerCreateInfo(createInfo);

      if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &debugMessenger) != VK_SUCCESS)
      {

         throw std::runtime_error("failed to set up debug messenger!");

      }

   }


   bool VkcDevice::checkValidationLayerSupport()
   {
      uint32_t layerCount;
      vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

      std::vector<VkLayerProperties> availableLayers(layerCount);
      vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

      for (const char* layerName : validationLayers)
      {
         bool layerFound = false;


         printf_line("checking for validation layer : %s", layerName);
         for (const auto& layerProperties : availableLayers)
         {
            printf_line("an available layer : %s", layerProperties.layerName);
            if (strcmp(layerName, layerProperties.layerName) == 0)
            {
               layerFound = true;
               break;
            }
         }

         if (!layerFound) {
            return false;
         }
      }

      return true;

   }


   std::vector<const char*> VkcDevice::getRequiredExtensions()
   {
      uint32_t glfwExtensionCount = 0;
      const char** glfwExtensions;
      //glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

      //std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
      std::vector<const char*> extensions;

      if (enableValidationLayers)
      {
         extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      return extensions;
      return {};
   }


   void VkcDevice::hasGflwRequiredInstanceExtensions()
   {
      uint32_t extensionCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
      std::vector<VkExtensionProperties> extensions(extensionCount);
      vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

      std::cout << "available extensions:" << std::endl;
      std::unordered_set<std::string> available;
      for (const auto& extension : extensions) {
         std::cout << "\t" << extension.extensionName << std::endl;
         available.insert(extension.extensionName);
      }

      std::cout << "required extensions:" << std::endl;
      auto requiredExtensions = getRequiredExtensions();
      for (const auto& required : requiredExtensions)
      {
         std::cout << "\t" << required << std::endl;
         if (available.find(required) == available.end()) {
            throw std::runtime_error("Missing required glfw extension");
         }
      }

   }


   bool VkcDevice::checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice)
   {

      uint32_t extensionCount;
      vkEnumerateDeviceExtensionProperties(pvkcdevice, nullptr, &extensionCount, nullptr);

      std::vector<VkExtensionProperties> availableExtensions(extensionCount);
      vkEnumerateDeviceExtensionProperties(
         pvkcdevice,
         nullptr,
         &extensionCount,
         availableExtensions.data());

      std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

      for (const auto& extension : availableExtensions) {
         requiredExtensions.erase(extension.extensionName);
      }

      return requiredExtensions.empty();
   }


   QueueFamilyIndices VkcDevice::findQueueFamilies(VkPhysicalDevice pvkcdevice)
   {

      QueueFamilyIndices indices;

      uint32_t queueFamilyCount = 0;
      vkGetPhysicalDeviceQueueFamilyProperties(pvkcdevice, &queueFamilyCount, nullptr);

      std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
      vkGetPhysicalDeviceQueueFamilyProperties(pvkcdevice, &queueFamilyCount, queueFamilies.data());

      int i = 0;
      for (const auto& queueFamily : queueFamilies)
      {
         if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
         {
            indices.graphicsFamily = i;
            indices.graphicsFamilyHasValue = true;
         }
         if (surface_)
         {
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(pvkcdevice, i, surface_, &presentSupport);
            if (queueFamily.queueCount > 0 && presentSupport)
            {
               indices.presentFamily = i;
               indices.presentFamilyHasValue = true;
            }
         }
         if (indices.isComplete()) {
            break;
         }

         i++;
      }

      return indices;

   }


   SwapChainSupportDetails VkcDevice::querySwapChainSupport(VkPhysicalDevice pvkcdevice)
   {

      SwapChainSupportDetails details{};

      if (surface_)
      {

         vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pvkcdevice, surface_, &details.capabilities);

         uint32_t formatCount;
         vkGetPhysicalDeviceSurfaceFormatsKHR(pvkcdevice, surface_, &formatCount, nullptr);

         if (formatCount != 0) {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(pvkcdevice, surface_, &formatCount, details.formats.data());
         }

         uint32_t presentModeCount;
         vkGetPhysicalDeviceSurfacePresentModesKHR(pvkcdevice, surface_, &presentModeCount, nullptr);

         if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(
               pvkcdevice,
               surface_,
               &presentModeCount,
               details.presentModes.data());
         }

      }
      return details;
   }


   VkFormat VkcDevice::findSupportedFormat(
      const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
      for (VkFormat format : candidates) {
         VkFormatProperties props;
         vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

         if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
         }
         else if (
            tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
         }
      }
      throw std::runtime_error("failed to find supported format!");
   }


   uint32_t VkcDevice::findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
      VkPhysicalDeviceMemoryProperties memProperties;
      vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);
      for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
         if ((typeFilter & (1 << i)) &&
            (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
         }
      }

      throw std::runtime_error("failed to find suitable memory type!");
   }


   void VkcDevice::createBuffer(
      VkDeviceSize size,
      VkBufferUsageFlags usage,
      VkMemoryPropertyFlags properties,
      VkBuffer& buffer,
      VkDeviceMemory& bufferMemory)
   {

      VkBufferCreateInfo bufferInfo{};
      bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      bufferInfo.size = size;
      bufferInfo.usage = usage;
      bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      if (vkCreateBuffer(m_vkdevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
      {

         throw std::runtime_error("failed to create vertex buffer!");

      }

      VkMemoryRequirements memRequirements;
      vkGetBufferMemoryRequirements(m_vkdevice, buffer, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
      {

         throw std::runtime_error("failed to allocate vertex buffer memory!");

      }

      vkBindBufferMemory(m_vkdevice, buffer, bufferMemory, 0);

   }


   VkCommandBuffer VkcDevice::beginSingleTimeCommands()
   {

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = commandPool;
      allocInfo.commandBufferCount = 1;

      VkCommandBuffer commandBuffer;
      vkAllocateCommandBuffers(m_vkdevice, &allocInfo, &commandBuffer);

      VkCommandBufferBeginInfo beginInfo{};
      beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      vkBeginCommandBuffer(commandBuffer, &beginInfo);
      return commandBuffer;

   }


   void VkcDevice::endSingleTimeCommands(VkCommandBuffer commandBuffer)
   {

      vkEndCommandBuffer(commandBuffer);

      VkSubmitInfo submitInfo{};
      submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &commandBuffer;

      vkQueueSubmit(graphicsQueue_, 1, &submitInfo, VK_NULL_HANDLE);
      vkQueueWaitIdle(graphicsQueue_);

      vkFreeCommandBuffers(m_vkdevice, commandPool, 1, &commandBuffer);

   }


   void VkcDevice::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
   {
      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkBufferCopy copyRegion{};
      copyRegion.srcOffset = 0;  // Optional
      copyRegion.dstOffset = 0;  // Optional
      copyRegion.size = size;
      vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

      endSingleTimeCommands(commandBuffer);

   }


   void VkcDevice::copyBufferToImage(
      VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount)
   {

      VkCommandBuffer commandBuffer = beginSingleTimeCommands();

      VkBufferImageCopy region{};
      region.bufferOffset = 0;
      region.bufferRowLength = 0;
      region.bufferImageHeight = 0;

      region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      region.imageSubresource.mipLevel = 0;
      region.imageSubresource.baseArrayLayer = 0;
      region.imageSubresource.layerCount = layerCount;

      region.imageOffset = { 0, 0, 0 };
      region.imageExtent = { width, height, 1 };

      vkCmdCopyBufferToImage(
         commandBuffer,
         buffer,
         image,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         1,
         &region);

      endSingleTimeCommands(commandBuffer);

   }


   void VkcDevice::createImageWithInfo(
      const VkImageCreateInfo& imageInfo,
      VkMemoryPropertyFlags properties,
      VkImage& image,
      VkDeviceMemory& imageMemory)
   {

      if (vkCreateImage(m_vkdevice, &imageInfo, nullptr, &image) != VK_SUCCESS)
      {

         throw std::runtime_error("failed to create image!");

      }

      VkMemoryRequirements memRequirements;
      vkGetImageMemoryRequirements(m_vkdevice, image, &memRequirements);

      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize = memRequirements.size;
      allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

      if (vkAllocateMemory(m_vkdevice, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
         throw std::runtime_error("failed to allocate image memory!");
      }

      if (vkBindImageMemory(m_vkdevice, image, imageMemory, 0) != VK_SUCCESS) {
         throw std::runtime_error("failed to bind image memory!");
      }
   }



   void VkcDevice::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   {
      VkSubmitInfo submitInfo = vks::initializers::submitInfo();
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &cmdBuffer;
      //m_submitInfo.commandBufferCount = 1;
      //m_submitInfo.pCommandBuffers = &cmdBuffer;
      VkFenceCreateInfo fenceInfo = vks::initializers::fenceCreateInfo();
      VkFence fence;
      VK_CHECK_RESULT(vkCreateFence(m_vkdevice, &fenceInfo, nullptr, &fence));
      VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
      VK_CHECK_RESULT(vkWaitForFences(m_vkdevice, 1, &fence, VK_TRUE, UINT64_MAX));
      vkDestroyFence(m_vkdevice, fence, nullptr);
   }


} // namespace vkc



