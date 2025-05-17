#pragma once

//#include "window.h"
//#include "vulkan-graphics3d/graphics3d_vulkan/device.h"
#include "_vulkan.h"
// std lib headers
#include <string>
#include <vector>

namespace graphics3d_vulkan
{

   
   class VkContainer;

   struct SwapChainSupportDetails
   {
      VkSurfaceCapabilitiesKHR capabilities;
      std::vector<VkSurfaceFormatKHR> formats;
      std::vector<VkPresentModeKHR> presentModes;
   };

   struct QueueFamilyIndices {
      uint32_t graphicsFamily;
      uint32_t presentFamily;
      bool graphicsFamilyHasValue = false;
      bool presentFamilyHasValue = false;
      bool isComplete() { return graphicsFamilyHasValue && presentFamilyHasValue; }
   };

   class VkcDevice :
      virtual public ::particle {
   public:
#if defined(NDEBUG)
      const bool enableValidationLayers = false;
#else
      const bool enableValidationLayers = true;
#endif

      //VkcDevice(VkWindow& window);
      //VkcDevice(VkPhysicalDevice physicalDevice);
      //VkcDevice(vks::VulkanDevice * pdevice);
      VkcDevice();
      ~VkcDevice();

      virtual void initialize_device(::vkc::VkContainer* pvkcontainer);

      // Not copyable or movable
      VkcDevice(const VkcDevice&) = delete;
      void operator=(const VkcDevice&) = delete;
      //VkcDevice(vks::VulkanDevice&&) = delete;
      VkcDevice& operator=(VkcDevice&&) = delete;

      VkCommandPool getCommandPool() { return commandPool; }
      VkDevice device() { return m_vkdevice; }

      VkSurfaceKHR surface() { return surface_; }
      VkQueue graphicsQueue() { return graphicsQueue_; }
      VkQueue presentQueue() { return presentQueue_; }

      SwapChainSupportDetails getSwapChainSupport() { return querySwapChainSupport(physicalDevice); }
      uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
      QueueFamilyIndices findPhysicalQueueFamilies() { return findQueueFamilies(physicalDevice); }
      VkFormat findSupportedFormat(
         const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);

      // Buffer Helper Functions
      void createBuffer(
         VkDeviceSize size,
         VkBufferUsageFlags usage,
         VkMemoryPropertyFlags properties,
         VkBuffer& buffer,
         VkDeviceMemory& bufferMemory);
      VkCommandBuffer beginSingleTimeCommands();
      void endSingleTimeCommands(VkCommandBuffer commandBuffer);
      void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
      void copyBufferToImage(
         VkBuffer buffer, VkImage image, uint32_t width, uint32_t height, uint32_t layerCount);

      void createImageWithInfo(
         const VkImageCreateInfo& imageInfo,
         VkMemoryPropertyFlags properties,
         VkImage& image,
         VkDeviceMemory& imageMemory);

      VkPhysicalDeviceProperties properties;


      void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);

   public:
      void createInstance();
      void setupDebugMessenger();
      void createSurface();
      void pickPhysicalDevice();
      void createLogicalDevice();
      void createCommandPool();

      // helper functions
      bool isDeviceSuitable(VkPhysicalDevice pvkcdevice);
      std::vector<const char*> getRequiredExtensions();
      bool checkValidationLayerSupport();
      QueueFamilyIndices findQueueFamilies(VkPhysicalDevice pvkcdevice);
      void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
      void hasGflwRequiredInstanceExtensions();
      bool checkDeviceExtensionSupport(VkPhysicalDevice pvkcdevice);
      SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice pvkcdevice);

      VkInstance instance;
      VkDebugUtilsMessengerEXT debugMessenger;
      VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
      ::pointer < vkc::VkContainer > m_pvkcontainer;
      VkCommandPool commandPool;

      VkDevice m_vkdevice;
      VkSurfaceKHR surface_;
      VkQueue graphicsQueue_;
      VkQueue presentQueue_;

      const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
      const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

   };


}  // namespace graphics3d_vulkan


