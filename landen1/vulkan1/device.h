/*
 * Vulkan device class
 *
 * Encapsulates a physical Vulkan device and its logical representation
 *
 * Copyright (C) 2016-2023 by Sascha Willems - www.saschawillems.de
 *
 * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
 */
#pragma once


#include "buffer.h"
#include "tools.h"
#include "_vulkan.h"
#include <algorithm>
#include <assert.h>
#include <exception>


namespace vulkan1
{


   struct device
   {
      /** @brief Physical device representation */
      VkPhysicalDevice m_vkphysicaldevice;
      /** @brief Logical device representation (application's view of the device) */
      VkDevice logicalDevice;
      /** @brief Properties of the physical device including limits that the application can check against */
      VkPhysicalDeviceProperties properties;
      /** @brief Features of the physical device that an application can use to check if a feature is supported */
      VkPhysicalDeviceFeatures features;
      /** @brief Features that have been enabled for use on the physical device */
      VkPhysicalDeviceFeatures m_vkphysicaldevicefeaturesEnabled;
      /** @brief Memory types and heaps of the physical device */
      VkPhysicalDeviceMemoryProperties memoryProperties;
      /** @brief Queue family properties of the physical device */
      std::vector<VkQueueFamilyProperties> queueFamilyProperties;
      /** @brief List of extensions supported by the device */
      std::vector<std::string> supportedExtensions;
      /** @brief Default command pool for the graphics m_vkqueue family index */
      VkCommandPool commandPool = VK_NULL_HANDLE;
      /** @brief Contains m_vkqueue family indices */
      struct
      {
         uint32_t graphics;
         uint32_t compute;
         uint32_t transfer;
      } queueFamilyIndices;
      operator VkDevice() const
      {
         return logicalDevice;
      };
      explicit device(VkPhysicalDevice m_vkphysicaldevice);
      ~device();
      uint32_t        getMemoryType(uint32_t typeBits, VkMemoryPropertyFlags properties, VkBool32* memTypeFound = nullptr) const;
      uint32_t        getQueueFamilyIndex(VkQueueFlags queueFlags) const;
      VkResult        createLogicalDevice(VkPhysicalDeviceFeatures m_vkphysicaldevicefeaturesEnabled, ::comparable_array<const char*> enabledExtensions, void* pNextChain, bool useSwapChain = true, VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
      VkResult        createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkDeviceSize size, VkBuffer* buffer, VkDeviceMemory* memory, void* data = nullptr);
      VkResult        createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, vulkan::buffer* buffer, VkDeviceSize size, void* data = nullptr);
      void            copyBuffer(vulkan::buffer* src, vulkan::buffer* dst, VkQueue m_vkqueue, VkBufferCopy* copyRegion = nullptr);
      VkCommandPool   createCommandPool(uint32_t queueFamilyIndex, VkCommandPoolCreateFlags createFlags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
      VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, VkCommandPool pool, bool begin = false);
      VkCommandBuffer createCommandBuffer(VkCommandBufferLevel level, bool begin = false);
      void            flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue m_vkqueue, VkCommandPool pool, bool free = true);
      void            flushCommandBuffer(VkCommandBuffer commandBuffer, VkQueue m_vkqueue, bool free = true);
      bool            extensionSupported(std::string extension);
      VkFormat        getSupportedDepthFormat(bool checkSamplingSupport);
   };


} // namespace vulkan1



