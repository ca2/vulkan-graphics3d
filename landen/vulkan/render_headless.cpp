/*
* Vulkan Example - Minimal headless rendering example
*
* Copyright (C) 2017 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "framework.h"
#include "render_headless.h"
#include "acme/_operating_system.h"
#include "VulkanTools.h"
#include "benchmark.hpp"
#include "camera.hpp"
//#include "VulkanSwapChain.h"
#include "VulkanglTFModel.h"
#include "VulkanUIOverlay.h"
#include "shared.h"
#define FB_DIM 512
#define FB_COLOR_FORMAT VK_FORMAT_R8G8B8A8_UNORM

#if defined(_WIN32)
#pragma comment(linker, "/subsystem:console"_ansi)
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
#include <android/native_activity.h>
#include <android/asset_manager.h>
#include <android_native_app_glue.h>
#include <android/log.h>
#include "VulkanAndroid.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <array>
#include <iostream>
#include <algorithm>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vulkan/vulkan.h>
#include "VulkanTools.h"

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
android_app * androidapp;
#endif

//#define DEBUG (!NDEBUG)

#define BUFFER_ELEMENTS 32

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
#define LOG(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vulkanExample"_ansi, __VA_ARGS__))
#else
#define LOG(...) printf(__VA_ARGS__)
#endif

static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
   VkDebugReportFlagsEXT flags,
   VkDebugReportObjectTypeEXT objectType,
   uint64_t object,
   size_t location,
   int32_t messageCode,
   const char * pLayerPrefix,
   const char * pMessage,
   void * pUserData)
{
   LOG("[VALIDATION]: %s - %s\n"_ansi, pLayerPrefix, pMessage);
   return VK_FALSE;
}

#define FB_DIM 512
#define FB_COLOR_FORMAT VK_FORMAT_R8G8B8A8_UNORM

VulkanExample5_base::VulkanExample5_base(mouseState * pmousestate)
{
   m_pmousestate = pmousestate;

}

VulkanExample5::VulkanExample5(mouseState * pmousestate):
VulkanExample5_base(pmousestate)
{
   //m_pmousestate = pmousestate;
   LOG("Running headless rendering example\n"_ansi);
   m_strApplicationName = "Vulken"_ansi;
   m_strEngineName = "vulken"_ansi;
   //
   //#if defined(VK_USE_PLATFORM_ANDROID_KHR)
   //      LOG("loading vulkan lib"_ansi);
   //      vks::android::loadVulkanLibrary();
   //#endif
   //
   //      VkApplicationInfo appInfo = {};
   //      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
   //      appInfo.pApplicationName = "Vulkan headless example"_ansi;
   //      appInfo.pEngineName = "VulkanExample5"_ansi;
   //      appInfo.apiVersion = VK_API_VERSION_1_0;
   //
   //      /*
   //         Vulkan instance creation (without surface extensions)
   //      */
   //      VkInstanceCreateInfo instanceCreateInfo = {};
   //      instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
   //      instanceCreateInfo.pApplicationInfo = &appInfo;
   //
   //      uint32_t layerCount = 0;
   //#if defined(VK_USE_PLATFORM_ANDROID_KHR)
   //      const char * validationLayers[] = { "VK_LAYER_GOOGLE_threading"_ansi,	"VK_LAYER_LUNARG_parameter_validation"_ansi,	"VK_LAYER_LUNARG_object_tracker"_ansi,"VK_LAYER_LUNARG_core_validation"_ansi,	"VK_LAYER_LUNARG_swapchain"_ansi, "VK_LAYER_GOOGLE_unique_objects"_ansi };
   //      layerCount = 6;
   //#else
   //      const char * validationLayers[] = { "VK_LAYER_LUNARG_standard_validation"_ansi };
   //      layerCount = 1;
   //#endif
   //#if DEBUG
   //      // Check if layers are available
   //      uint32_t instanceLayerCount;
   //      vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
   //      std::vector<VkLayerProperties> instanceLayers(instanceLayerCount);
   //      vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayers.data());
   //
   //      bool layersAvailable = true;
   //      for (auto layerName : validationLayers) {
   //         bool layerAvailable = false;
   //         for (auto instanceLayer : instanceLayers) {
   //            if (strcmp(instanceLayer.layerName, layerName) == 0) {
   //               layerAvailable = true;
   //               break;
   //            }
   //         }
   //         if (!layerAvailable) {
   //            layersAvailable = false;
   //            break;
   //         }
   //      }
   //
   //      if (layersAvailable) {
   //         instanceCreateInfo.ppEnabledLayerNames = validationLayers;
   //         const char * validationExt = VK_EXT_DEBUG_REPORT_EXTENSION_NAME;
   //         instanceCreateInfo.enabledLayerCount = layerCount;
   //         instanceCreateInfo.enabledExtensionCount = 1;
   //         instanceCreateInfo.ppEnabledExtensionNames = &validationExt;
   //      }
   //#endif
   //      VK_CHECK_RESULT(vkCreateInstance(&instanceCreateInfo, nullptr, &instance));
   //
   //#if defined(VK_USE_PLATFORM_ANDROID_KHR)
   //      vks::android::loadVulkanFunctions(instance);
   //#endif
   //#if DEBUG
   //      if (layersAvailable) {
   //         VkDebugReportCallbackCreateInfoEXT debugReportCreateInfo = {};
   //         debugReportCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
   //         debugReportCreateInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
   //         debugReportCreateInfo.pfnCallback = (PFN_vkDebugReportCallbackEXT)debugMessageCallback;
   //
   //         // We have to explicitly load this function.
   //         PFN_vkCreateDebugReportCallbackEXT vkCreateDebugReportCallbackEXT = reinterpret_cast<PFN_vkCreateDebugReportCallbackEXT>(vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT"_ansi));
   //         assert(vkCreateDebugReportCallbackEXT);
   //         VK_CHECK_RESULT(vkCreateDebugReportCallbackEXT(instance, &debugReportCreateInfo, nullptr, &debugReportCallback));
   //      }
   //#endif
   //
   //      /*
   //         Vulkan device creation
   //      */
   //      uint32_t deviceCount = 0;
   //      VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
   //      std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
   //      VK_CHECK_RESULT(vkEnumeratePhysicalDevices(instance, &deviceCount, physicalDevices.data()));
   //      physicalDevice = physicalDevices[0];
   //
   //      //VkPhysicalDeviceProperties deviceProperties;
   //      vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
   //      LOG("GPU: %s\n"_ansi, deviceProperties.deviceName);

         //// Request a single graphics queue
         //const float defaultQueuePriority(0.0f);
         //VkDeviceQueueCreateInfo queueCreateInfo = {};
         //uint32_t queueFamilyCount;
         //vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
         ////std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyCount);
         //vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyProperties.data());
         //for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamilyProperties.size()); i++) {
         //   if (queueFamilyProperties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
         //      queueFamilyIndex = i;
         //      queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
         //      queueCreateInfo.queueFamilyIndex = i;
         //      queueCreateInfo.queueCount = 1;
         //      queueCreateInfo.pQueuePriorities = &defaultQueuePriority;
         //      break;
         //   }
         //}
         // Create logical device
         //VkDeviceCreateInfo deviceCreateInfo = {};
         //deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
         //deviceCreateInfo.queueCreateInfoCount = 1;
         //deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;
         //VK_CHECK_RESULT(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device));

         // Get a graphics queue
         //vkGetDeviceQueue(device, queueFamilyIndex, 0, &queue);

         //// Command pool
         //VkCommandPoolCreateInfo cmdPoolInfo = {};
         //cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
         //cmdPoolInfo.queueFamilyIndex = queueFamilyIndex;
         //cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
         //VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &commandPool));

         /*
            Prepare vertex and index buffers
         */
         /*struct Vertex {
            float position[3];
            float color[3];
         };
         {
            std::vector<Vertex> vertices = {
               { {  1.0f,  1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f } },
               { { -1.0f,  1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f } },
               { {  0.0f, -1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f } }
            };
            std::vector<uint32_t> indices = { 0, 1, 2 };

            const VkDeviceSize vertexBufferSize = vertices.size() * sizeof(Vertex);
            const VkDeviceSize indexBufferSize = indices.size() * sizeof(uint32_t);

            VkBuffer stagingBuffer;
            VkDeviceMemory stagingMemory;*/

            //// Command buffer for copy commands (reused)
            //VkCommandBufferAllocateInfo cmdBufAllocateInfo = vks::initializers::commandBufferAllocateInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
            //VkCommandBuffer copyCmd;
            //VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &copyCmd));
            //VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

         //   // Copy input data to VRAM using a staging buffer
         //   {
         //      // Vertices
         //      createBuffer(
         //         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         //         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         //         &stagingBuffer,
         //         &stagingMemory,
         //         vertexBufferSize,
         //         vertices.data());

         //      createBuffer(
         //         VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         //         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
         //         &vertexBuffer,
         //         &vertexMemory,
         //         vertexBufferSize);

         //      VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));
         //      VkBufferCopy copyRegion = {};
         //      copyRegion.size = vertexBufferSize;
         //      vkCmdCopyBuffer(copyCmd, stagingBuffer, vertexBuffer, 1, &copyRegion);
         //      VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

         //      submitWork(copyCmd, queue);

         //      vkDestroyBuffer(device, stagingBuffer, nullptr);
         //      vkFreeMemory(device, stagingMemory, nullptr);

         //      // Indices
         //      createBuffer(
         //         VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
         //         VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
         //         &stagingBuffer,
         //         &stagingMemory,
         //         indexBufferSize,
         //         indices.data());

         //      createBuffer(
         //         VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
         //         VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
         //         &indexBuffer,
         //         &indexMemory,
         //         indexBufferSize);

         //      VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));
         //      copyRegion.size = indexBufferSize;
         //      vkCmdCopyBuffer(copyCmd, stagingBuffer, indexBuffer, 1, &copyRegion);
         //      VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

         //      submitWork(copyCmd, queue);

         //      vkDestroyBuffer(device, stagingBuffer, nullptr);
         //      vkFreeMemory(device, stagingMemory, nullptr);
         //   }
         //}

         ///*
         //   Create framebuffer attachments
         //*/
         ////m_width = 1024;
         ////m_height = 1024;
         //VkFormat colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
         //VkFormat depthFormat;
         //vks::tools::getSupportedDepthFormat(physicalDevice, &depthFormat);
         //{
         //   // Color attachment
         //   VkImageCreateInfo image = vks::initializers::imageCreateInfo();
         //   image.imageType = VK_IMAGE_TYPE_2D;
         //   image.format = colorFormat;
         //   image.extent.width = m_width;
         //   image.extent.height = m_height;
         //   image.extent.depth = 1;
         //   image.mipLevels = 1;
         //   image.arrayLayers = 1;
         //   image.samples = VK_SAMPLE_COUNT_1_BIT;
         //   image.tiling = VK_IMAGE_TILING_OPTIMAL;
         //   image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

         //   VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
         //   VkMemoryRequirements memReqs;

         //   VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &colorAttachment.image));
         //   vkGetImageMemoryRequirements(device, colorAttachment.image, &memReqs);
         //   memAlloc.allocationSize = memReqs.size;
         //   memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         //   VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &colorAttachment.mem));
         //   VK_CHECK_RESULT(vkBindImageMemory(device, colorAttachment.image, colorAttachment.mem, 0));

         //   VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
         //   colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
         //   colorImageView.format = colorFormat;
         //   colorImageView.subresourceRange = {};
         //   colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         //   colorImageView.subresourceRange.baseMipLevel = 0;
         //   colorImageView.subresourceRange.levelCount = 1;
         //   colorImageView.subresourceRange.baseArrayLayer = 0;
         //   colorImageView.subresourceRange.layerCount = 1;
         //   colorImageView.image = colorAttachment.image;
         //   VK_CHECK_RESULT(vkCreateImageView(device, &colorImageView, nullptr, &colorAttachment.view));

         //   // Depth stencil attachment
         //   image.format = depthFormat;
         //   image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

         //   VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &depthAttachment.image));
         //   vkGetImageMemoryRequirements(device, depthAttachment.image, &memReqs);
         //   memAlloc.allocationSize = memReqs.size;
         //   memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         //   VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &depthAttachment.mem));
         //   VK_CHECK_RESULT(vkBindImageMemory(device, depthAttachment.image, depthAttachment.mem, 0));

         //   VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
         //   depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
         //   depthStencilView.format = depthFormat;
         //   depthStencilView.flags = 0;
         //   depthStencilView.subresourceRange = {};
         //   depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
         //   depthStencilView.subresourceRange.baseMipLevel = 0;
         //   depthStencilView.subresourceRange.levelCount = 1;
         //   depthStencilView.subresourceRange.baseArrayLayer = 0;
         //   depthStencilView.subresourceRange.layerCount = 1;
         //   depthStencilView.image = depthAttachment.image;
         //   VK_CHECK_RESULT(vkCreateImageView(device, &depthStencilView, nullptr, &depthAttachment.view));
         //}

         ///*
         //   Create renderpass
         //*/
         //{
         //   std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};
         //   // Color attachment
         //   attchmentDescriptions[0].format = colorFormat;
         //   attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
         //   attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
         //   attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
         //   attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
         //   attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
         //   attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //   attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
         //   // Depth attachment
         //   attchmentDescriptions[1].format = depthFormat;
         //   attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
         //   attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
         //   attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
         //   attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
         //   attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
         //   attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
         //   attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

         //   VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
         //   VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

         //   VkSubpassDescription subpassDescription = {};
         //   subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
         //   subpassDescription.colorAttachmentCount = 1;
         //   subpassDescription.pColorAttachments = &colorReference;
         //   subpassDescription.pDepthStencilAttachment = &depthReference;

         //   // Use subpass dependencies for layout transitions
         //   std::array<VkSubpassDependency, 2> dependencies;

         //   dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
         //   dependencies[0].dstSubpass = 0;
         //   dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         //   dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         //   dependencies[0].srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         //   dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         //   dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

         //   dependencies[1].srcSubpass = 0;
         //   dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
         //   dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
         //   dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
         //   dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
         //   dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
         //   dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

         //   // Create the actual renderpass
         //   VkRenderPassCreateInfo renderPassInfo = {};
         //   renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
         //   renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
         //   renderPassInfo.pAttachments = attchmentDescriptions.data();
         //   renderPassInfo.subpassCount = 1;
         //   renderPassInfo.pSubpasses = &subpassDescription;
         //   renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
         //   renderPassInfo.pDependencies = dependencies.data();
         //   VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &renderPass));

         //   VkImageView attachments[2];
         //   attachments[0] = colorAttachment.view;
         //   attachments[1] = depthAttachment.view;

         //   VkFramebufferCreateInfo framebufferCreateInfo = vks::initializers::framebufferCreateInfo();
         //   framebufferCreateInfo.renderPass = renderPass;
         //   framebufferCreateInfo.attachmentCount = 2;
         //   framebufferCreateInfo.pAttachments = attachments;
         //   framebufferCreateInfo.width = m_width;
         //   framebufferCreateInfo.height = m_height;
         //   framebufferCreateInfo.layers = 1;
         //   VK_CHECK_RESULT(vkCreateFramebuffer(device, &framebufferCreateInfo, nullptr, &framebuffer));
         //}

         // TODO : Draw the glTF Model here.

   //      /*
   //         Prepare graphics pipeline
   //      */
   //      {
   //         std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings = {};
   //         VkDescriptorSetLayoutCreateInfo descriptorLayout =
   //            vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
   //         VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayout, nullptr, &descriptorSetLayout));
   //
   //         VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo =
   //            vks::initializers::pipelineLayoutCreateInfo(nullptr, 0);
   //
   //         // MVP via push constant block
   //         VkPushConstantRange pushConstantRange = vks::initializers::pushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(glm::mat4), 0);
   //         pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
   //         pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
   //
   //         VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout));
   //
   //         VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
   //         pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
   //         VK_CHECK_RESULT(vkCreatePipelineCache(device, &pipelineCacheCreateInfo, nullptr, &pipelineCache));
   //
   //         // Create pipeline
   //         VkPipelineInputAssemblyStateCreateInfo inputAssemblyState =
   //            vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
   //
   //         VkPipelineRasterizationStateCreateInfo rasterizationState =
   //            vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE);
   //
   //         VkPipelineColorBlendAttachmentState blendAttachmentState =
   //            vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
   //
   //         VkPipelineColorBlendStateCreateInfo colorBlendState =
   //            vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
   //
   //         VkPipelineDepthStencilStateCreateInfo depthStencilState =
   //            vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
   //
   //         VkPipelineViewportStateCreateInfo viewportState =
   //            vks::initializers::pipelineViewportStateCreateInfo(1, 1);
   //
   //         VkPipelineMultisampleStateCreateInfo multisampleState =
   //            vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);
   //
   //         std::vector<VkDynamicState> dynamicStateEnables = {
   //            VK_DYNAMIC_STATE_VIEWPORT,
   //            VK_DYNAMIC_STATE_SCISSOR
   //         };
   //         VkPipelineDynamicStateCreateInfo dynamicState =
   //            vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);
   //
   //         VkGraphicsPipelineCreateInfo pipelineCreateInfo =
   //            vks::initializers::pipelineCreateInfo(pipelineLayout, renderPass);
   //
   //         std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages{};
   //
   //         pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
   //         pipelineCreateInfo.pRasterizationState = &rasterizationState;
   //         pipelineCreateInfo.pColorBlendState = &colorBlendState;
   //         pipelineCreateInfo.pMultisampleState = &multisampleState;
   //         pipelineCreateInfo.pViewportState = &viewportState;
   //         pipelineCreateInfo.pDepthStencilState = &depthStencilState;
   //         pipelineCreateInfo.pDynamicState = &dynamicState;
   //         pipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
   //         pipelineCreateInfo.pStages = shaderStages.data();
   //
   //         // Vertex bindings an attributes
   //         // Binding description
   //         std::vector<VkVertexInputBindingDescription> vertexInputBindings = {
   //            vks::initializers::vertexInputBindingDescription(0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX),
   //         };
   //
   //         // Attribute descriptions
   //         std::vector<VkVertexInputAttributeDescription> vertexInputAttributes = {
   //            vks::initializers::vertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0),					// Position
   //            vks::initializers::vertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32B32_SFLOAT, sizeof(float) * 3),	// Color
   //         };
   //
   //         VkPipelineVertexInputStateCreateInfo vertexInputState = vks::initializers::pipelineVertexInputStateCreateInfo();
   //         vertexInputState.vertexBindingDescriptionCount = static_cast<uint32_t>(vertexInputBindings.size());
   //         vertexInputState.pVertexBindingDescriptions = vertexInputBindings.data();
   //         vertexInputState.vertexAttributeDescriptionCount = static_cast<uint32_t>(vertexInputAttributes.size());
   //         vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes.data();
   //
   //         pipelineCreateInfo.pVertexInputState = &vertexInputState;
   //
   //         // TODO: There is no command line arguments parsing (nor Android settings) for this
   //         // example, so we have no way of picking between GLSL or HLSL shaders.
   //         // Hard-code to glsl for now.
   //         const std::string shadersPath = getAssetPath() + "shaders/glsl/renderheadless/"_ansi;
   //
   //         shaderStages[0].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
   //         shaderStages[0].stage = VK_SHADER_STAGE_VERTEX_BIT;
   //         shaderStages[0].pName = "main"_ansi;
   //         shaderStages[1].sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
   //         shaderStages[1].stage = VK_SHADER_STAGE_FRAGMENT_BIT;
   //         shaderStages[1].pName = "main"_ansi;
   //#if defined(VK_USE_PLATFORM_ANDROID_KHR)
   //         shaderStages[0].module = vks::tools::loadShader(androidapp->activity->assetManager, (shadersPath + "triangle.vert.spv"_ansi).c_str(), device);
   //         shaderStages[1].module = vks::tools::loadShader(androidapp->activity->assetManager, (shadersPath + "triangle.frag.spv"_ansi).c_str(), device);
   //#else
   //         shaderStages[0].module = vks::tools::loadShader((shadersPath + "triangle.vert.spv"_ansi).c_str(), device);
   //         shaderStages[1].module = vks::tools::loadShader((shadersPath + "triangle.frag.spv"_ansi).c_str(), device);
   //#endif
   //         shaderModules = { shaderStages[0].module, shaderStages[1].module };
   //         VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCreateInfo, nullptr, &pipeline));
   //      }

         ///*
         //   Command buffer creation
         //*/
         //{
         //   VkCommandBuffer commandBuffer;
         //   VkCommandBufferAllocateInfo cmdBufAllocateInfo =
         //      vks::initializers::commandBufferAllocateInfo(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
         //   VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &commandBuffer));

         //   VkCommandBufferBeginInfo cmdBufInfo =
         //      vks::initializers::commandBufferBeginInfo();

         //   VK_CHECK_RESULT(vkBeginCommandBuffer(commandBuffer, &cmdBufInfo));

         //   VkClearValue clearValues[2];
         //   clearValues[0].color = { { 0.0f, 0.0f, 0.2f, 1.0f } };
         //   clearValues[1].depthStencil = { 1.0f, 0 };

         //   VkRenderPassBeginInfo renderPassBeginInfo = {};
         //   renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
         //   renderPassBeginInfo.renderArea.extent.width = m_width;
         //   renderPassBeginInfo.renderArea.extent.height = m_height;
         //   renderPassBeginInfo.clearValueCount = 2;
         //   renderPassBeginInfo.pClearValues = clearValues;
         //   renderPassBeginInfo.renderPass = renderPass;
         //   renderPassBeginInfo.framebuffer = framebuffer;

         //   vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

         //   VkViewport viewport = {};
         //   viewport.height = (float)m_height;
         //   viewport.width = (float)m_width;
         //   viewport.minDepth = (float)0.0f;
         //   viewport.maxDepth = (float)1.0f;
         //   vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

         //   // Update dynamic scissor state
         //   VkRect2D scissor = {};
         //   scissor.extent.width = m_width;
         //   scissor.extent.height = m_height;
         //   vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

         //   vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

         //   // Render scene
         //   VkDeviceSize offsets[1] = { 0 };
         //   vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vertexBuffer, offsets);
         //   vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

         //   std::vector<glm::vec3> pos = {
         //      glm::vec3(-1.5f, 0.0f, -4.0f),
         //      glm::vec3(0.0f, 0.0f, -2.5f),
         //      glm::vec3(1.5f, 0.0f, -4.0f),
         //   };

         //   for (auto v : pos) {
         //      glm::mat4 mvpMatrix = glm::perspective(glm::radians(60.0f), (float)m_width / (float)m_height, 0.1f, 256.0f) * glm::translate(glm::mat4(1.0f), v);
         //      vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvpMatrix), &mvpMatrix);
         //      vkCmdDrawIndexed(commandBuffer, 3, 1, 0, 0, 0);
         //   }

         //   vkCmdEndRenderPass(commandBuffer);

         //   VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));

         //   submitWork(commandBuffer, queue);

         //   vkDeviceWaitIdle(device);
         //}


}


VulkanExample5_base::~VulkanExample5_base()
{

   if (m_device)
   {


      //if (m_device) {
         // Frame buffer

         // Color attachment
         vkDestroyImageView(m_device, m_offscreenPass.color.view, nullptr);
         vkDestroyImage(m_device, m_offscreenPass.color.image, nullptr);
         vkFreeMemory(m_device, m_offscreenPass.color.mem, nullptr);

         // Depth attachment
         vkDestroyImageView(m_device, m_offscreenPass.depth.view, nullptr);
         vkDestroyImage(m_device, m_offscreenPass.depth.image, nullptr);
         vkFreeMemory(m_device, m_offscreenPass.depth.mem, nullptr);

         vkDestroyRenderPass(m_device, m_offscreenPass.renderPass, nullptr);
         vkDestroySampler(m_device, m_offscreenPass.sampler, nullptr);
         vkDestroyFramebuffer(m_device, m_offscreenPass.frameBuffer, nullptr);

         vkDestroyPipeline(m_device, m_pipelines.debug, nullptr);
         vkDestroyPipeline(m_device, m_pipelines.shaded, nullptr);
         vkDestroyPipeline(m_device, m_pipelines.shadedOffscreen, nullptr);
         vkDestroyPipeline(m_device, m_pipelines.mirror, nullptr);

         vkDestroyPipelineLayout(m_device, m_pipelineLayouts.textured, nullptr);
         vkDestroyPipelineLayout(m_device, m_pipelineLayouts.shaded, nullptr);

         vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayouts.shaded, nullptr);
         vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayouts.textured, nullptr);

         // Uniform buffers
         m_uniformBuffers.vsShared.destroy();
         m_uniformBuffers.vsMirror.destroy();
         m_uniformBuffers.vsOffScreen.destroy();

      vkDestroyBuffer(m_device, m_vertexBuffer, nullptr);
      vkFreeMemory(m_device, m_vertexMemory, nullptr);
      vkDestroyBuffer(m_device, m_indexBuffer, nullptr);
      vkFreeMemory(m_device, m_indexMemory, nullptr);
      vkDestroyImageView(m_device, m_colorAttachment.view, nullptr);
      vkDestroyImage(m_device, m_colorAttachment.image, nullptr);
      vkFreeMemory(m_device, m_colorAttachment.mem, nullptr);
      vkDestroyImageView(m_device, m_depthAttachment.view, nullptr);
      vkDestroyImage(m_device, m_depthAttachment.image, nullptr);
      vkFreeMemory(m_device, m_depthAttachment.mem, nullptr);
      vkDestroyRenderPass(m_device, m_renderPass, nullptr);
      vkDestroyFramebuffer(m_device, m_framebuffer, nullptr);
      vkDestroyPipelineLayout(m_device, m_pipelineLayout, nullptr);
      vkDestroyDescriptorSetLayout(m_device, m_descriptorSetLayout, nullptr);
      vkDestroyPipeline(m_device, m_pipeline, nullptr);
      vkDestroyPipelineCache(m_device, m_pipelineCache, nullptr);
      //vkDestroyCommandPool(device, commandPool, nullptr);
      for (auto shadermodule : m_shaderModules) {
         vkDestroyShaderModule(m_device, shadermodule, nullptr);
      }
      vkDestroyDevice(m_device, nullptr);

   }
#if DEBUG
   if (m_debugReportCallback) {
      PFN_vkDestroyDebugReportCallbackEXT vkDestroyDebugReportCallback = reinterpret_cast<PFN_vkDestroyDebugReportCallbackEXT>(vkGetInstanceProcAddr(m_instance, "vkDestroyDebugReportCallbackEXT"_ansi));
      assert(vkDestroyDebugReportCallback);
      vkDestroyDebugReportCallback(m_instance, m_debugReportCallback, nullptr);
   }
#endif
   vkDestroyInstance(m_instance, nullptr);
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
   vks::android::freeVulkanLibrary();
#endif
}

VulkanExample5::~VulkanExample5()
{

}

   VkResult VulkanExample5_base::createInstance()
   {
      std::vector<const char *> instanceExtensions = { VK_KHR_SURFACE_EXTENSION_NAME };

      // Enable surface extensions depending on os
#if defined(_WIN32)
      instanceExtensions.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
      instanceExtensions.push_back(VK_KHR_ANDROID_SURFACE_EXTENSION_NAME);
#elif defined(_DIRECT2DISPLAY)
      instanceExtensions.push_back(VK_KHR_DISPLAY_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
      instanceExtensions.push_back(VK_EXT_DIRECTFB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
      instanceExtensions.push_back(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_XCB_KHR)
      instanceExtensions.push_back(VK_KHR_XCB_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_IOS_MVK)
      instanceExtensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_MACOS_MVK)
      instanceExtensions.push_back(VK_MVK_MACOS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_METAL_EXT)
      instanceExtensions.push_back(VK_EXT_METAL_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
      instanceExtensions.push_back(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME);
#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
      instanceExtensions.push_back(VK_QNX_SCREEN_SURFACE_EXTENSION_NAME);
#endif

      // Get extensions supported by the instance and store for later use
      uint32_t extCount = 0;
      vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
      if (extCount > 0)
      {
         std::vector<VkExtensionProperties> extensions(extCount);
         if (vkEnumerateInstanceExtensionProperties(nullptr, &extCount, &extensions.front()) == VK_SUCCESS)
         {
            for (VkExtensionProperties & extension : extensions)
            {
               m_supportedInstanceExtensions.push_back(extension.extensionName);
            }
         }
      }

#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
      // SRS - When running on iOS/macOS with MoltenVK, enable VK_KHR_get_physical_device_properties2 if not already enabled by the example (required by VK_KHR_portability_subset)
      if (std::find(enabledInstanceExtensions.begin(), enabledInstanceExtensions.end(), VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) == enabledInstanceExtensions.end())
      {
         enabledInstanceExtensions.push_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
      }
#endif

      // Enabled requested instance extensions
      if (m_enabledInstanceExtensions.size() > 0)
      {
         for (const char * enabledExtension : m_enabledInstanceExtensions)
         {
            // Output message if requested extension is not available
            if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), enabledExtension) == m_supportedInstanceExtensions.end())
            {
               std::cerr << "Enabled instance extension \""_ansi << enabledExtension << "\" is not present at instance level\n"_ansi;
            }
            instanceExtensions.push_back(enabledExtension);
         }
      }

      VkApplicationInfo appInfo{};
      appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
      appInfo.pApplicationName = m_strApplicationName;
      appInfo.pEngineName = m_strEngineName;
      appInfo.apiVersion = m_apiVersion;

      VkInstanceCreateInfo instanceCreateInfo{};
      instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
      instanceCreateInfo.pApplicationInfo = &appInfo;

      VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCI{};
      if (m_settings.validation) {
         vks::debug::setupDebugingMessengerCreateInfo(debugUtilsMessengerCI);
         debugUtilsMessengerCI.pNext = instanceCreateInfo.pNext;
         instanceCreateInfo.pNext = &debugUtilsMessengerCI;
      }

#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && defined(VK_KHR_portability_enumeration)
      // SRS - When running on iOS/macOS with MoltenVK and VK_KHR_portability_enumeration is defined and supported by the instance, enable the extension and the flag
      if (std::find(supportedInstanceExtensions.begin(), supportedInstanceExtensions.end(), VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME) != supportedInstanceExtensions.end())
      {
         instanceExtensions.push_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
         instanceCreateInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
      }
#endif

      // Enable the debug utils extension if available (e.g. when debugging tools are present)
      if (m_settings.validation || std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) != m_supportedInstanceExtensions.end()) {
         instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      }

      if (instanceExtensions.size() > 0) {
         instanceCreateInfo.enabledExtensionCount = (uint32_t)instanceExtensions.size();
         instanceCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();
      }

      // The VK_LAYER_KHRONOS_validation contains all current validation functionality.
      // Note that on Android this layer requires at least NDK r20
      const char * validationLayerName = "VK_LAYER_KHRONOS_validation"_ansi;
      if (m_settings.validation) {
         // Check if this layer is available at instance level
         uint32_t instanceLayerCount;
         vkEnumerateInstanceLayerProperties(&instanceLayerCount, nullptr);
         std::vector<VkLayerProperties> instanceLayerProperties(instanceLayerCount);
         vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayerProperties.data());
         bool validationLayerPresent = false;
         for (VkLayerProperties & layer : instanceLayerProperties) {
            if (strcmp(layer.layerName, validationLayerName) == 0) {
               validationLayerPresent = true;
               break;
            }
         }
         if (validationLayerPresent) {
            instanceCreateInfo.ppEnabledLayerNames = &validationLayerName;
            instanceCreateInfo.enabledLayerCount = 1;
         }
         else {
            std::cerr << "Validation layer VK_LAYER_KHRONOS_validation not present, validation is disabled"_ansi;
         }
      }
      VkResult result = vkCreateInstance(&instanceCreateInfo, nullptr, &m_instance);

      // If the debug utils extension is present we set up debug functions, so samples can label objects for debugging
      if (std::find(m_supportedInstanceExtensions.begin(), m_supportedInstanceExtensions.end(), VK_EXT_DEBUG_UTILS_EXTENSION_NAME) != m_supportedInstanceExtensions.end()) {
         vks::debugutils::setup(m_instance);
      }

      return result;
   }

   
   void VulkanExample5_base::getEnabledFeatures()
   {
   

   }

   
   void VulkanExample5_base::getEnabledExtensions()
   {
   

   }


   bool VulkanExample5_base::initVulkan()
   {
      // Instead of checking for the command line switch, validation can be forced via a define
#if defined(_VALIDATION)
      this->settings.validation = true;
#endif

      // Create the instance
      VkResult result = createInstance();
      if (result != VK_SUCCESS) {
         vks::tools::exitFatal("Could not create Vulkan instance : \n" + vks::tools::errorString(result), result);
         return false;
      }

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
      vks::android::loadVulkanFunctions(instance);
#endif

      // If requested, we enable the default validation layers for debugging
      if (m_settings.validation)
      {
         vks::debug::setupDebugging(m_instance);
      }

      // Physical device
      uint32_t gpuCount = 0;
      // Get number of available physical devices
      VK_CHECK_RESULT(vkEnumeratePhysicalDevices(m_instance, &gpuCount, nullptr));
      if (gpuCount == 0) {
         vks::tools::exitFatal("No device with Vulkan support found", -1);
         return false;
      }
      // Enumerate devices
      std::vector<VkPhysicalDevice> physicalDevices(gpuCount);
      result = vkEnumeratePhysicalDevices(m_instance, &gpuCount, physicalDevices.data());
      if (result != VK_SUCCESS) {
         vks::tools::exitFatal("Could not enumerate physical devices : \n" + vks::tools::errorString(result), result);
         return false;
      }

      // GPU selection

      // Select physical device to be used for the Vulkan example
      // Defaults to the first device unless specified by command line
      uint32_t selectedDevice = 0;

      //#if !defined(VK_USE_PLATFORM_ANDROID_KHR)
      //	// GPU selection via command line argument
      //	if (commandLineParser.isSet("gpuselection"_ansi)) {
      //		uint32_t index = commandLineParser.getValueAsInt("gpuselection"_ansi, 0);
      //		if (index > gpuCount - 1) {
      //			std::cerr << "Selected device index "_ansi << index << " is out of range, reverting to device 0 (use -listgpus to show available Vulkan devices)"_ansi << "\n"_ansi;
      //		} else {
      //			selectedDevice = index;
      //		}
      //	}
      //	if (commandLineParser.isSet("gpulist"_ansi)) {
      //		std::cout << "Available Vulkan devices"_ansi << "\n"_ansi;
      //		for (uint32_t i = 0; i < gpuCount; i++) {
      //			VkPhysicalDeviceProperties deviceProperties;
      //			vkGetPhysicalDeviceProperties(physicalDevices[i], &deviceProperties);
      //			std::cout << "Device ["_ansi << i << "] : "_ansi << deviceProperties.deviceName << std::endl;
      //			std::cout << " Type: "_ansi << vks::tools::physicalDeviceTypeString(deviceProperties.deviceType) << "\n"_ansi;
      //			std::cout << " API: "_ansi << (deviceProperties.apiVersion >> 22) << "."_ansi << ((deviceProperties.apiVersion >> 12) & 0x3ff) << "."_ansi << (deviceProperties.apiVersion & 0xfff) << "\n"_ansi;
      //		}
      //	}
      //#endif
      //
      m_physicalDevice = physicalDevices[selectedDevice];

      // Store properties (including limits), features and memory properties of the physical device (so that examples can check against them)
      vkGetPhysicalDeviceProperties(m_physicalDevice, &m_deviceProperties);
      vkGetPhysicalDeviceFeatures(m_physicalDevice, &m_deviceFeatures);
      vkGetPhysicalDeviceMemoryProperties(m_physicalDevice, &m_deviceMemoryProperties);

      // Derived examples can override this to set actual features (based on above readings) to enable for logical device creation
      getEnabledFeatures();

      // Vulkan device creation
      // This is handled by a separate class that gets a logical device representation
      // and encapsulates functions related to a device
      m_pvulkandevice = ___new vks::VulkanDevice(m_physicalDevice);

      // Derived examples can enable extensions based on the list of supported extensions read from the physical device
      getEnabledExtensions();

      result = m_pvulkandevice->createLogicalDevice(m_enabledFeatures, m_enabledDeviceExtensions, m_deviceCreatepNextChain);
      if (result != VK_SUCCESS) {
         vks::tools::exitFatal("Could not create Vulkan device: \n" + vks::tools::errorString(result), result);
         return false;
      }
      m_device = m_pvulkandevice->logicalDevice;

      // Get a graphics queue from the device
      vkGetDeviceQueue(m_device, m_pvulkandevice->queueFamilyIndices.graphics, 0, &m_queue);

      // Find a suitable depth and/or stencil format
      VkBool32 validFormat{ false };
      // Samples that make use of stencil will require a depth + stencil format, so we select from a different list
      if (m_requiresStencil) {
         validFormat = vks::tools::getSupportedDepthStencilFormat(m_physicalDevice, &m_depthFormat);
      }
      else {
         validFormat = vks::tools::getSupportedDepthFormat(m_physicalDevice, &m_depthFormat);
      }
      assert(validFormat);

      //m_swapchain.setContext(instance, physicalDevice, device);

      //// Create synchronization objects
      //VkSemaphoreCreateInfo semaphoreCreateInfo = vks::initializers::semaphoreCreateInfo();
      //// Create a semaphore used to synchronize image presentation
      //// Ensures that the image is displayed before we start submitting ___new commands to the queue
      //VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.presentComplete));
      //// Create a semaphore used to synchronize command submission
      //// Ensures that the image is not presented until all commands have been submitted and executed
      //VK_CHECK_RESULT(vkCreateSemaphore(m_device, &semaphoreCreateInfo, nullptr, &m_semaphores.renderComplete));

      // Set up submit info structure
      // Semaphores will stay the same during application lifetime
      // Command buffer submission info is set by each example
      //m_submitInfo = vks::initializers::submitInfo();
      //m_submitInfo.pWaitDstStageMask = &m_submitPipelineStages;
      //m_submitInfo.waitSemaphoreCount = 1;
      //m_submitInfo.pWaitSemaphores = &m_semaphores.presentComplete;
      //m_submitInfo.signalSemaphoreCount = 1;
      //m_submitInfo.pSignalSemaphores = &m_semaphores.renderComplete;

      return true;
   }


   //uint32_t getMemoryTypeIndex(uint32_t typeBits, VkMemoryPropertyFlags properties) {
   //   //VkPhysicalDeviceMemoryProperties deviceMemoryProperties;
   //   vkGetPhysicalDeviceMemoryProperties(physicalDevice, &deviceMemoryProperties);
   //   for (uint32_t i = 0; i < deviceMemoryProperties.memoryTypeCount; i++) {
   //      if ((typeBits & 1) == 1) {
   //         if ((deviceMemoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
   //            return i;
   //         }
   //      }
   //      typeBits >>= 1;
   //   }
   //   return 0;
   //}

   //VkResult createBuffer(VkBufferUsageFlags usageFlags, VkMemoryPropertyFlags memoryPropertyFlags, VkBuffer * buffer, VkDeviceMemory * memory, VkDeviceSize size, void * data = nullptr)
   //{
   //   // Create the buffer handle
   //   VkBufferCreateInfo bufferCreateInfo = vks::initializers::bufferCreateInfo(usageFlags, size);
   //   bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
   //   VK_CHECK_RESULT(vkCreateBuffer(device, &bufferCreateInfo, nullptr, buffer));

   //   // Create the memory backing up the buffer handle
   //   VkMemoryRequirements memReqs;
   //   VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
   //   vkGetBufferMemoryRequirements(device, *buffer, &memReqs);
   //   memAlloc.allocationSize = memReqs.size;
   //   memAlloc.memoryTypeIndex = getMemoryTypeIndex(memReqs.memoryTypeBits, memoryPropertyFlags);
   //   VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, memory));

   //   if (data != nullptr) {
   //      void * mapped;
   //      VK_CHECK_RESULT(vkMapMemory(device, *memory, 0, size, 0, &mapped));
   //      memcpy(mapped, data, size);
   //      vkUnmapMemory(device, *memory);
   //   }

   //   VK_CHECK_RESULT(vkBindBufferMemory(device, *buffer, *memory, 0));

   //   return VK_SUCCESS;
   //}

   /*
      Submit command buffer to a queue and wait for fence until queue operations have been finished
   */
   void VulkanExample5_base::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
   {
      VkSubmitInfo submitInfo = vks::initializers::submitInfo();
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &cmdBuffer;
      //m_submitInfo.commandBufferCount = 1;
      //m_submitInfo.pCommandBuffers = &cmdBuffer;
      VkFenceCreateInfo fenceInfo = vks::initializers::fenceCreateInfo();
      VkFence fence;
      VK_CHECK_RESULT(vkCreateFence(m_pvulkandevice->logicalDevice, &fenceInfo, nullptr, &fence));
      VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
      VK_CHECK_RESULT(vkWaitForFences(m_pvulkandevice->logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX));
      vkDestroyFence(m_pvulkandevice->logicalDevice, fence, nullptr);
   }

   

   void VulkanExample5_base::renderLoop2(const ::function < void(void *, int, int, int)> & callback)
   {
      // SRS - for non-apple plaforms, handle benchmarking here within VulkanExampleBase::renderLoop()
      //     - for macOS, handle benchmarking within NSApp rendering loop via displayLinkOutputCb()
#if !(defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
      if (m_benchmark.active) {
#if defined(VK_USE_PLATFORM_WAYLAND_KHR)
         while (!configured)
            wl_display_dispatch(display);
         while (wl_display_prepare_read(display) != 0)
            wl_display_dispatch_pending(display);
         wl_display_flush(display);
         wl_display_read_events(display);
         wl_display_dispatch_pending(display);
#endif

         m_benchmark.run([=] { render(callback); }, m_pvulkandevice->properties);
         vkDeviceWaitIdle(m_device);
         if (m_benchmark.filename != "") {
            m_benchmark.saveResults();
         }
         return;
      }
#endif

      //destWidth = width;
      //destHeight = height;
      m_lastTimestamp = std::chrono::high_resolution_clock::now();
      m_tPrevEnd = m_lastTimestamp;
#if defined(_WIN32)
      //MSG msg;
      //bool quitMessageReceived = false;
      //while (!quitMessageReceived) {
      //   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
      //      TranslateMessage(&msg);
      //      DispatchMessage(&msg);
      //      if (msg.message == WM_QUIT) {
      //         quitMessageReceived = true;
      //         break;
      //      }
      //   }
      //   if (prepared && !IsIconic(window)) {
      while (::task_get_run())
      {
         nextFrame(callback);
         //void * pData = nullptr;
         //vkMapMemory(m_pvulkandevice->logicalDevice, offscreenPass.color.mem, 0, m_width * m_height * 4, VK_MEMORY_MAP_PLACED_BIT_EXT, &pData);
         //callback(pData, m_width, m_height, m_width * 4);
         //vkUnmapMemory(m_pvulkandevice->logicalDevice, offscreenPass.color.mem);
         preempt(20_ms);
      }
#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
      while (1)
      {
         int ident;
         int happenings;
         struct android_poll_source * source;
         bool destroy = false;

         focused = true;

         while ((ident = ALooper_pollAll(focused ? 0 : -1, NULL, &happenings, (void **)&source)) >= 0)
         {
            if (source != NULL)
            {
               source->process(androidApp, source);
            }
            if (androidApp->destroyRequested != 0)
            {
               LOGD("Android app destroy requested"_ansi);
               destroy = true;
               break;
            }
         }

         // App destruction requested
         // Exit loop, example will be destroyed in application main
         if (destroy)
         {
            ANativeActivity_finish(androidApp->activity);
            break;
         }

         // Render frame
         if (prepared)
         {
            auto tStart = std::chrono::high_resolution_clock::now();
            render();
            frameCounter++;
            auto tEnd = std::chrono::high_resolution_clock::now();
            auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
            frameTimer = tDiff / 1000.0f;
            camera.update(frameTimer);
            // Convert to clamped timer value
            if (!paused)
            {
               timer += timerSpeed * frameTimer;
               if (timer > 1.0)
               {
                  timer -= 1.0f;
               }
            }
            float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
            if (fpsTimer > 1000.0f)
            {
               lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
               frameCounter = 0;
               lastTimestamp = tEnd;
            }

            updateOverlay();

            bool updateView = false;

            // Check touch state (for movement)
            if (touchDown) {
               touchTimer += frameTimer;
            }
            if (touchTimer >= 1.0) {
               camera.keys.up = true;
            }

            // Check gamepad state
            const float deadZone = 0.0015f;
            if (camera.type != Camera::CameraType::firstperson)
            {
               // Rotate
               if (std::abs(gamePadState.axisLeft.x) > deadZone)
               {
                  camera.rotate(glm::vec3(0.0f, gamePadState.axisLeft.x * 0.5f, 0.0f));
                  updateView = true;
               }
               if (std::abs(gamePadState.axisLeft.y) > deadZone)
               {
                  camera.rotate(glm::vec3(gamePadState.axisLeft.y * 0.5f, 0.0f, 0.0f));
                  updateView = true;
               }
               // Zoom
               if (std::abs(gamePadState.axisRight.y) > deadZone)
               {
                  camera.translate(glm::vec3(0.0f, 0.0f, gamePadState.axisRight.y * 0.01f));
                  updateView = true;
               }
            }
            else
            {
               updateView = camera.updatePad(gamePadState.axisLeft, gamePadState.axisRight, frameTimer);
            }
         }
      }
#elif defined(_DIRECT2DISPLAY)
      while (!quit)
      {
         auto tStart = std::chrono::high_resolution_clock::now();
         if (viewUpdated)
         {
            viewUpdated = false;
         }
         render();
         frameCounter++;
         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         frameTimer = tDiff / 1000.0f;
         camera.update(frameTimer);
         if (camera.moving())
         {
            viewUpdated = true;
         }
         // Convert to clamped timer value
         if (!paused)
         {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
               timer -= 1.0f;
            }
         }
         float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
         if (fpsTimer > 1000.0f)
         {
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
         }
         updateOverlay();
      }
#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
      while (!quit)
      {
         auto tStart = std::chrono::high_resolution_clock::now();
         if (viewUpdated)
         {
            viewUpdated = false;
         }
         DFBWindowEvent happening;
         while (!event_buffer->GetEvent(event_buffer, DFB_EVENT(&happening)))
         {
            handleEvent(&happening);
         }
         render();
         frameCounter++;
         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         frameTimer = tDiff / 1000.0f;
         camera.update(frameTimer);
         if (camera.moving())
         {
            viewUpdated = true;
         }
         // Convert to clamped timer value
         if (!paused)
         {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
               timer -= 1.0f;
            }
         }
         float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
         if (fpsTimer > 1000.0f)
         {
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
         }
         updateOverlay();
      }
#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
      while (!quit)
      {
         auto tStart = std::chrono::high_resolution_clock::now();
         if (viewUpdated)
         {
            viewUpdated = false;
         }

         while (!configured)
            wl_display_dispatch(display);
         while (wl_display_prepare_read(display) != 0)
            wl_display_dispatch_pending(display);
         wl_display_flush(display);
         wl_display_read_events(display);
         wl_display_dispatch_pending(display);

         render();
         frameCounter++;
         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         frameTimer = tDiff / 1000.0f;
         camera.update(frameTimer);
         if (camera.moving())
         {
            viewUpdated = true;
         }
         // Convert to clamped timer value
         if (!paused)
         {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
               timer -= 1.0f;
            }
         }
         float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
         if (fpsTimer > 1000.0f)
         {
            if (!settings.overlay)
            {
               std::string windowTitle = getWindowTitle();
               xdg_toplevel_set_title(xdg_toplevel, windowTitle.c_str());
            }
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
         }
         updateOverlay();
      }
#elif defined(VK_USE_PLATFORM_XCB_KHR)
      xcb_flush(connection);
      while (!quit)
      {
         auto tStart = std::chrono::high_resolution_clock::now();
         if (viewUpdated)
         {
            viewUpdated = false;
         }
         xcb_generic_event_t * happening;
         while ((happening = xcb_poll_for_event(connection)))
         {
            handleEvent(happening);
            free(happening);
         }
         render();
         frameCounter++;
         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         frameTimer = tDiff / 1000.0f;
         camera.update(frameTimer);
         if (camera.moving())
         {
            viewUpdated = true;
         }
         // Convert to clamped timer value
         if (!paused)
         {
            timer += timerSpeed * frameTimer;
            if (timer > 1.0)
            {
               timer -= 1.0f;
            }
         }
         float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
         if (fpsTimer > 1000.0f)
         {
            if (!settings.overlay)
            {
               std::string windowTitle = getWindowTitle();
               xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
                  window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
                  windowTitle.size(), windowTitle.c_str());
            }
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
         }
         updateOverlay();
      }
#elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
      while (!quit)
      {
         auto tStart = std::chrono::high_resolution_clock::now();
         if (viewUpdated)
         {
            viewUpdated = false;
         }
         render();
         frameCounter++;
         auto tEnd = std::chrono::high_resolution_clock::now();
         auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
         frameTimer = tDiff / 1000.0f;
         camera.update(frameTimer);
         if (camera.moving())
         {
            viewUpdated = true;
         }
         // Convert to clamped timer value
         timer += timerSpeed * frameTimer;
         if (timer > 1.0)
         {
            timer -= 1.0f;
         }
         float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
         if (fpsTimer > 1000.0f)
         {
            lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
            frameCounter = 0;
            lastTimestamp = tEnd;
         }
         updateOverlay();
      }
#elif (defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && defined(VK_EXAMPLE_XCODE_GENERATED)
      [NSApp run];
#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
      while (!quit) {
         handleEvent();

         if (prepared) {
            nextFrame();
         }
      }
#endif
      // Flush device to make sure all resources can be freed
      if (m_device != VK_NULL_HANDLE) {
         vkDeviceWaitIdle(m_device);
      }
   }


      void VulkanExample5_base::sample(const ::function < void(void *, int, int, int)> & callback)
      {


         /*
            Copy framebuffer image to host visible image
         */
         const char * imagedata;
         {
            // Create the linear tiled destination image to copy to and to read the memory from
            VkImageCreateInfo imgCreateInfo(vks::initializers::imageCreateInfo());
            imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imgCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            imgCreateInfo.extent.width = m_width;
            imgCreateInfo.extent.height = m_height;
            imgCreateInfo.extent.depth = 1;
            imgCreateInfo.arrayLayers = 1;
            imgCreateInfo.mipLevels = 1;
            imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
            imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            // Create the image
            VkImage dstImage;
            VK_CHECK_RESULT(vkCreateImage(m_device, &imgCreateInfo, nullptr, &dstImage));
            // Create memory to back up the image
            VkMemoryRequirements memRequirements;
            VkMemoryAllocateInfo memAllocInfo(vks::initializers::memoryAllocateInfo());
            VkDeviceMemory dstImageMemory;
            vkGetImageMemoryRequirements(m_device, dstImage, &memRequirements);
            memAllocInfo.allocationSize = memRequirements.size;
            // Memory must be host visible to copy from
            memAllocInfo.memoryTypeIndex = m_pvulkandevice->getMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
            VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAllocInfo, nullptr, &dstImageMemory));
            VK_CHECK_RESULT(vkBindImageMemory(m_device, dstImage, dstImageMemory, 0));

            // Do the actual blit from the offscreen image to our host visible destination image
            VkCommandBufferAllocateInfo cmdBufAllocateInfo = vks::initializers::commandBufferAllocateInfo(m_commandPoolTransfer, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
            VkCommandBuffer copyCmd;
            VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, &copyCmd));
            VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
            VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

            // Transition destination image to transfer destination layout
            vks::tools::insertImageMemoryBarrier(
               copyCmd,
               dstImage,
               0,
               VK_ACCESS_TRANSFER_WRITE_BIT,
               VK_IMAGE_LAYOUT_UNDEFINED,
               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
               VK_PIPELINE_STAGE_TRANSFER_BIT,
               VK_PIPELINE_STAGE_TRANSFER_BIT,
               VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

            VkImageCopy imageCopyRegion{};
            imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageCopyRegion.srcSubresource.layerCount = 1;
            imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            imageCopyRegion.dstSubresource.layerCount = 1;
            imageCopyRegion.extent.width = m_width;
            imageCopyRegion.extent.height = m_height;
            imageCopyRegion.extent.depth = 1;

            vkCmdCopyImage(
               copyCmd,
               //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
               m_colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
               dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
               1,
               &imageCopyRegion);

            // Transition destination image to general layout, which is the required layout for mapping the image memory later on
            vks::tools::insertImageMemoryBarrier(
               copyCmd,
               dstImage,
               VK_ACCESS_TRANSFER_WRITE_BIT,
               VK_ACCESS_MEMORY_READ_BIT,
               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
               VK_IMAGE_LAYOUT_GENERAL,
               VK_PIPELINE_STAGE_TRANSFER_BIT,
               VK_PIPELINE_STAGE_TRANSFER_BIT,
               VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

            submitWork(copyCmd, m_queue);

         // Get layout of the image (including row pitch)
         VkImageSubresource subResource{};
         subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
         VkSubresourceLayout subResourceLayout;

         vkGetImageSubresourceLayout(m_device, dstImage, &subResource, &subResourceLayout);

         // Map image memory so we can start copying from it
         vkMapMemory(m_device, dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void **)&imagedata);
         imagedata += subResourceLayout.offset;

         /*
            Save host visible framebuffer image to disk (ppm format)
         */

         //::memory mem;

         //mem.set_size(m_width * m_height * 4);

         //std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
         //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

         callback((void *)imagedata, m_width, m_height, subResourceLayout.rowPitch);

         //// ppm binary pixel data
         //for (int32_t y = 0; y < m_height; y++) {
         //   unsigned int * row = (unsigned int *)imagedata;
         //   for (int32_t x = 0; x < m_width; x++) {
         //      //               if (colorSwizzle) {
         //         file.write((char *)row + 2, 1);
         //         file.write((char *)row + 1, 1);
         //         file.write((char *)row, 1);
         //      }
         //      //else {
         //      //   file.write((char *)row, 3);
         //      //}
         //      row++;
         //   }
         //   imagedata += subResourceLayout.rowPitch;
         //}


//         callback

//#if defined (VK_USE_PLATFORM_ANDROID_KHR)
//         const char * filename = strcat(getenv("EXTERNAL_STORAGE"_ansi), "/headless.ppm"_ansi);
//#else
//         const char * filename = "headless.ppm"_ansi;
//#endif
//         std::ofstream file(filename, std::ios::out | std::ios::binary);
//
//         // ppm header
//         file << "P6\n"_ansi << m_width << "\n"_ansi << m_height << "\n"_ansi << 255 << "\n"_ansi;

         //// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'hi have to manually swizzle color components
         //// Check if source is BGR and needs swizzle
         //std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
         //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

         //// ppm binary pixel data
         //for (int32_t y = 0; y < m_height; y++) {
         //   unsigned int * row = (unsigned int *)imagedata;
         //   for (int32_t x = 0; x < m_width; x++) {
         //      if (colorSwizzle) {
         //         file.write((char *)row + 2, 1);
         //         file.write((char *)row + 1, 1);
         //         file.write((char *)row, 1);
         //      }
         //      else {
         //         file.write((char *)row, 3);
         //      }
         //      row++;
         //   }
         //   imagedata += subResourceLayout.rowPitch;
         //}
         //file.close();

         //LOG("Framebuffer image saved to %s\n"_ansi, filename);

         // Clean up resources
         vkUnmapMemory(m_device, dstImageMemory);
         vkFreeMemory(m_device, dstImageMemory, nullptr);
         vkDestroyImage(m_device, dstImage, nullptr);
      }

      vkQueueWaitIdle(m_queue);

      }

      /** @brief Adds the drawing commands for the ImGui overlay to the given command buffer */
      void VulkanExample5_base::drawUI(const VkCommandBuffer commandBuffer)
      {

         if (m_settings.overlay && m_ui.visible) {
            const VkViewport viewport = vks::initializers::viewport((float)m_width, (float)m_height, 0.0f, 1.0f);
            const VkRect2D scissor = vks::initializers::rect2D(m_width, m_height, 0, 0);
            vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
            vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

            m_ui.draw(commandBuffer);
         }

      }


   void VulkanExample5_base::buildCommandBuffers()
   {
   }


   void VulkanExample5::buildCommandBuffers()
   {
      VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

      for (int32_t i = 0; i < m_drawCmdBuffers.size(); ++i)
      {
         VK_CHECK_RESULT(vkBeginCommandBuffer(m_drawCmdBuffers[i], &cmdBufInfo));

         /*
            First render pass: Offscreen rendering
         */
         {
            VkClearValue clearValues[2];
            clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
            clearValues[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = m_offscreenPass.renderPass;
            renderPassBeginInfo.framebuffer = m_offscreenPass.frameBuffer;
            renderPassBeginInfo.renderArea.extent.width = m_offscreenPass.width;
            renderPassBeginInfo.renderArea.extent.height = m_offscreenPass.height;
            renderPassBeginInfo.clearValueCount = 2;
            renderPassBeginInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport = vks::initializers::viewport((float)m_offscreenPass.width, (float)m_offscreenPass.height, 0.0f, 1.0f);
            vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

            VkRect2D scissor = vks::initializers::rect2D(m_offscreenPass.width, m_offscreenPass.height, 0, 0);
            vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

            // Mirrored scene
            vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayouts.shaded, 0, 1, &m_descriptorSets.offscreen, 0, NULL);
            vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelines.shadedOffscreen);
            m_models.example.draw(m_drawCmdBuffers[i]);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
         }

         /*
            Note: Explicit synchronization is not required between the render pass, as this is done implicit via sub pass dependencies
         */

         /*
            Second render pass: Scene rendering with applied radial blur
         */
         {
            VkClearValue clearValues[2];
            clearValues[0].color = m_defaultClearColor;
            clearValues[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = m_renderPass;
            renderPassBeginInfo.framebuffer = m_frameBuffers[i];
            renderPassBeginInfo.renderArea.extent.width = m_width;
            renderPassBeginInfo.renderArea.extent.height = m_height;
            renderPassBeginInfo.clearValueCount = 2;
            renderPassBeginInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport = vks::initializers::viewport((float)m_width, (float)m_height, 0.0f, 1.0f);
            vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

            VkRect2D scissor = vks::initializers::rect2D(m_width, m_height, 0, 0);
            vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

            if (m_debugDisplay)
            {
               // Display the offscreen render target
               vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayouts.textured, 0, 1, &m_descriptorSets.mirror, 0, nullptr);
               vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelines.debug);
               vkCmdDraw(m_drawCmdBuffers[i], 3, 1, 0, 0);
            }
            else {
               // Render the scene
               // Reflection plane
               vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayouts.textured, 0, 1, &m_descriptorSets.mirror, 0, nullptr);
               vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelines.mirror);
               m_models.plane.draw(m_drawCmdBuffers[i]);
               // Model
               vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLayouts.shaded, 0, 1, &m_descriptorSets.model, 0, nullptr);
               vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelines.shaded);
               m_models.example.draw(m_drawCmdBuffers[i]);
            }

            drawUI(m_drawCmdBuffers[i]);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
         }

         VK_CHECK_RESULT(vkEndCommandBuffer(m_drawCmdBuffers[i]));
      }
   }

   void VulkanExample5_base::nextFrame(const ::function < void(void *, int, int, int)> & callback)
   {
      auto tStart = std::chrono::high_resolution_clock::now();
      if (m_viewUpdated)
      {
         m_viewUpdated = false;
      }

      render(callback);
      m_frameCounter++;
      auto tEnd = std::chrono::high_resolution_clock::now();
#if (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && !defined(VK_EXAMPLE_XCODE_GENERATED)
      // SRS - Calculate tDiff as time between frames vs. rendering time for iOS/macOS displayLink-driven examples project
      auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tPrevEnd).count();
#else
      auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
#endif
      m_frameTimer = (float)tDiff / 1000.0f;
      m_camera.update(m_frameTimer);
      if (m_camera.moving())
      {
         m_viewUpdated = true;
      }
      // Convert to clamped timer value
      if (!m_paused)
      {
         m_fTimer += m_fTimerSpeed * m_frameTimer;
         if (m_fTimer > 1.0)
         {
            m_fTimer -= 1.0f;
         }
      }
      float fpsTimer = (float)(std::chrono::duration<double, std::milli>(tEnd - m_lastTimestamp).count());
      if (fpsTimer > 1000.0f)
      {
         m_lastFPS = static_cast<uint32_t>((float)m_frameCounter * (1000.0f / fpsTimer));
//#if defined(_WIN32)
//         if (!settings.overlay) {
//            std::string windowTitle = getWindowTitle();
//            SetWindowText(window, ::wstring(windowTitle.c_str()));
//         }
//#endif
         m_frameCounter = 0;
         m_lastTimestamp = tEnd;
      }
      m_tPrevEnd = tEnd;

      updateOverlay();
   }
   void VulkanExample5_base::OnUpdateUIOverlay(vks::UIOverlay * overlay)
   {
      if (overlay->header("Settings"_ansi)) {
         if (overlay->checkBox("Display render target"_ansi, &m_debugDisplay)) {
            buildCommandBuffers();
         }
      }
   }

   void VulkanExample5_base::updateOverlay()
   {
      if (!m_settings.overlay)
         return;

      // The overlay does not need to be updated with each frame, so we limit the update rate
      // Not only does this save performance but it also makes display of fast changig values like fps more stable
      m_ui.updateTimer -= m_frameTimer;
      if (m_ui.updateTimer >= 0.0f) {
         return;
      }
      // Update at max. rate of 30 fps
      m_ui.updateTimer = 1.0f / 30.0f;

      ImGuiIO & io = ImGui::GetIO();

      io.DisplaySize = ImVec2((float)m_width, (float)m_height);
      io.DeltaTime = m_frameTimer;

      io.MousePos = ImVec2(m_pmousestate->position.x, m_pmousestate->position.y);
      io.MouseDown[0] = m_pmousestate->buttons.left && m_ui.visible;
      io.MouseDown[1] = m_pmousestate->buttons.right && m_ui.visible;
      io.MouseDown[2] = m_pmousestate->buttons.middle && m_ui.visible;

      ImGui::NewFrame();

      ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0);
      ImGui::SetNextWindowPos(ImVec2(10 * m_ui.scale, 10 * m_ui.scale));
      ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);
      ImGui::Begin("Vulkan Example"_ansi, nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
      ImGui::TextUnformatted(m_strApplicationName);
      ImGui::TextUnformatted(m_deviceProperties.deviceName);
      ImGui::Text("%.2f ms/frame (%.1d fps)"_ansi, (1000.0f / m_lastFPS), m_lastFPS);

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
      ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, 5.0f * ui.scale));
#endif
      ImGui::PushItemWidth(110.0f * m_ui.scale);
      OnUpdateUIOverlay(&m_ui);
      ImGui::PopItemWidth();
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
      ImGui::PopStyleVar();
#endif

      ImGui::End();
      ImGui::PopStyleVar();
      ImGui::Render();

      if (m_ui.update() || m_ui.updated) {
         buildCommandBuffers();
         m_ui.updated = false;
      }

#if defined(VK_USE_PLATFORM_ANDROID_KHR)
      if (mouseState.buttons.left) {
         mouseState.buttons.left = false;
      }
#endif
   }


   void VulkanExample5_base::loadAssets()
   {
      

   }

   void VulkanExample5::loadAssets()
   {

      
         const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;

         ::vks::VulkanDevice * pdevice = m_pvulkandevice;

         ::file::path pathFolder(getAssetPath().c_str());

         auto pathPlaneGltf = pathFolder / "models/plane.gltf"_ansi;

         auto pathDragonGltf = pathFolder / "models/chinesedragon.gltf"_ansi;

         m_models.plane.loadFromFile(pathPlaneGltf, pdevice, m_queue, glTFLoadingFlags);

         m_models.example.loadFromFile(pathDragonGltf, pdevice, m_queue, glTFLoadingFlags);


   }


   void VulkanExample5_base::setupDescriptors()
   {
      // Pool
      std::vector<VkDescriptorPoolSize> poolSizes = {
         vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 6),
         vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 8)
      };
      VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, 5);
      VK_CHECK_RESULT(vkCreateDescriptorPool(m_device, &descriptorPoolInfo, nullptr, &m_descriptorPool));

      // Layout
      std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
      VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo;

      // Shaded layout
      setLayoutBindings = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
      };
      descriptorLayoutInfo = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
      VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device, &descriptorLayoutInfo, nullptr, &m_descriptorSetLayouts.shaded));

      // Textured layouts
      setLayoutBindings = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
         // Binding 1 : Fragment shader image sampler
         vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
      };
      descriptorLayoutInfo = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
      VK_CHECK_RESULT(vkCreateDescriptorSetLayout(m_device, &descriptorLayoutInfo, nullptr, &m_descriptorSetLayouts.textured));

      // Sets
      // Mirror plane descriptor set
      VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(m_descriptorPool, &m_descriptorSetLayouts.textured, 1);
      VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSets.mirror));
      std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::writeDescriptorSet(m_descriptorSets.mirror, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &m_uniformBuffers.vsMirror.descriptor),
         // Binding 1 : Fragment shader texture sampler
         vks::initializers::writeDescriptorSet(m_descriptorSets.mirror, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &m_offscreenPass.descriptor),
      };
      vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

      // Shaded descriptor sets
      allocInfo = vks::initializers::descriptorSetAllocateInfo(m_descriptorPool, &m_descriptorSetLayouts.shaded, 1);
      // Model
      VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSets.model));
      std::vector<VkWriteDescriptorSet> modelWriteDescriptorSets = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::writeDescriptorSet(m_descriptorSets.model, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &m_uniformBuffers.vsShared.descriptor)
      };
      vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(modelWriteDescriptorSets.size()), modelWriteDescriptorSets.data(), 0, nullptr);

      // Offscreen
      VK_CHECK_RESULT(vkAllocateDescriptorSets(m_device, &allocInfo, &m_descriptorSets.offscreen));
      std::vector<VkWriteDescriptorSet> offScreenWriteDescriptorSets = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::writeDescriptorSet(m_descriptorSets.offscreen, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &m_uniformBuffers.vsOffScreen.descriptor)
      };
      vkUpdateDescriptorSets(m_device, static_cast<uint32_t>(offScreenWriteDescriptorSets.size()), offScreenWriteDescriptorSets.data(), 0, nullptr);

   }

//   void initSwapchain()
//   {
//#if defined(_WIN32)
//      //TODO //m_swapchain.initSurface(windowInstance, window);
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//      swapChain.initSurface(androidApp->window);
//#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK))
//      swapChain.initSurface(view);
//#elif defined(VK_USE_PLATFORM_METAL_EXT)
//      swapChain.initSurface(metalLayer);
//#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
//      swapChain.initSurface(dfb, surface);
//#elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
//      swapChain.initSurface(display, surface);
//#elif defined(VK_USE_PLATFORM_XCB_KHR)
//      swapChain.initSurface(connection, window);
//#elif (defined(_DIRECT2DISPLAY) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
//      swapChain.initSurface(width, height);
//#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
//      swapChain.initSurface(screen_context, screen_window);
//#endif
//   }

   //void setupSwapChain()
   //{
   //   m_swapchain.create(&m_width, &m_height, settings.vsync, settings.fullscreen);
   //}


   //void createCommandPool()
   //{
   //   VkCommandPoolCreateInfo cmdPoolInfo = {};
   //   cmdPoolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
   //   cmdPoolInfo.queueFamilyIndex = m_swapchain.queueNodeIndex;
   //   cmdPoolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
   //   VK_CHECK_RESULT(vkCreateCommandPool(device, &cmdPoolInfo, nullptr, &cmdPool));
   //}


   void VulkanExample5_base::createTransferCommandPool()
   {
      // Create a default command pool for graphics command buffers
      m_commandPoolTransfer = m_pvulkandevice->createCommandPool(m_pvulkandevice->queueFamilyIndices.transfer);

   }


   void VulkanExample5_base::createCommandBuffers()
   {
      // Create one command buffer for each swap chain image
//      m_drawCmdBuffers.resize(m_swapchain.imageCount);
      m_drawCmdBuffers.resize(1);
      VkCommandBufferAllocateInfo cmdBufAllocateInfo = vks::initializers::commandBufferAllocateInfo(m_pvulkandevice->commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, static_cast<uint32_t>(m_drawCmdBuffers.size()));
      VK_CHECK_RESULT(vkAllocateCommandBuffers(m_device, &cmdBufAllocateInfo, m_drawCmdBuffers.data()));
   }

   void VulkanExample5_base::createSynchronizationPrimitives()
   {
      // Wait fences to sync command buffer access
      VkFenceCreateInfo fenceCreateInfo = vks::initializers::fenceCreateInfo(VK_FENCE_CREATE_SIGNALED_BIT);
      m_waitFences.resize(m_drawCmdBuffers.size());
      for (auto & fence : m_waitFences) {
         VK_CHECK_RESULT(vkCreateFence(m_device, &fenceCreateInfo, nullptr, &fence));
      }
   }
   void VulkanExample5_base::setupDepthStencil()
   {
      VkImageCreateInfo imageCI{};
      imageCI.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      imageCI.imageType = VK_IMAGE_TYPE_2D;
      imageCI.format = m_depthFormat;
      imageCI.extent = { m_width, m_height, 1 };
      imageCI.mipLevels = 1;
      imageCI.arrayLayers = 1;
      imageCI.samples = VK_SAMPLE_COUNT_1_BIT;
      imageCI.tiling = VK_IMAGE_TILING_OPTIMAL;
      imageCI.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

      VK_CHECK_RESULT(vkCreateImage(m_device, &imageCI, nullptr, &m_depthStencil.image));
      VkMemoryRequirements memReqs{};
      vkGetImageMemoryRequirements(m_device, m_depthStencil.image, &memReqs);

      VkMemoryAllocateInfo memAllloc{};
      memAllloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      memAllloc.allocationSize = memReqs.size;
      memAllloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAllloc, nullptr, &m_depthStencil.memory));
      VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depthStencil.image, m_depthStencil.memory, 0));

      VkImageViewCreateInfo imageViewCI{};
      imageViewCI.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      imageViewCI.viewType = VK_IMAGE_VIEW_TYPE_2D;
      imageViewCI.image = m_depthStencil.image;
      imageViewCI.format = m_depthFormat;
      imageViewCI.subresourceRange.baseMipLevel = 0;
      imageViewCI.subresourceRange.levelCount = 1;
      imageViewCI.subresourceRange.baseArrayLayer = 0;
      imageViewCI.subresourceRange.layerCount = 1;
      imageViewCI.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      // Stencil aspect should only be set on depth + stencil formats (VK_FORMAT_D16_UNORM_S8_UINT..VK_FORMAT_D32_SFLOAT_S8_UINT
      if (m_depthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
         imageViewCI.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
      VK_CHECK_RESULT(vkCreateImageView(m_device, &imageViewCI, nullptr, &m_depthStencil.view));
   }

   void VulkanExample5_base::setupRenderPass()
   {
      std::array<VkAttachmentDescription, 2> attachments = {};
      // Color attachment
      VkFormat colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
      //attachments[0].format = m_swapchain.colorFormat;
      attachments[0].format = colorFormat;
      attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
      attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
      // Depth attachment
      attachments[1].format = m_depthFormat;
      attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
      attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      VkAttachmentReference colorReference = {};
      colorReference.attachment = 0;
      colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

      VkAttachmentReference depthReference = {};
      depthReference.attachment = 1;
      depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      VkSubpassDescription subpassDescription = {};
      subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpassDescription.colorAttachmentCount = 1;
      subpassDescription.pColorAttachments = &colorReference;
      subpassDescription.pDepthStencilAttachment = &depthReference;
      subpassDescription.inputAttachmentCount = 0;
      subpassDescription.pInputAttachments = nullptr;
      subpassDescription.preserveAttachmentCount = 0;
      subpassDescription.pPreserveAttachments = nullptr;
      subpassDescription.pResolveAttachments = nullptr;

      // Subpass dependencies for layout transitions
      std::array<VkSubpassDependency, 2> dependencies;

      dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
      dependencies[0].dstSubpass = 0;
      dependencies[0].srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      dependencies[0].dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      dependencies[0].srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      dependencies[0].dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT;
      dependencies[0].dependencyFlags = 0;

      dependencies[1].srcSubpass = VK_SUBPASS_EXTERNAL;
      dependencies[1].dstSubpass = 0;
      dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependencies[1].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
      dependencies[1].srcAccessMask = 0;
      dependencies[1].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
      dependencies[1].dependencyFlags = 0;

      VkRenderPassCreateInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
      renderPassInfo.pAttachments = attachments.data();
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpassDescription;
      renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
      renderPassInfo.pDependencies = dependencies.data();

      VK_CHECK_RESULT(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_renderPass));
   }
   void VulkanExample5_base::createPipelineCache()
   {
      VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
      pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
      VK_CHECK_RESULT(vkCreatePipelineCache(m_device, &pipelineCacheCreateInfo, nullptr, &m_pipelineCache));
   }

   //VkImage m_imageRender[1];
   //VkDeviceMemory m_devicememoryRender[1];
   //VkImageView m_imageviewRender[1];

   void VulkanExample5_base::setupFrameBuffer()
   {


      // Find a suitable depth format
      VkFormat fbDepthFormat;
      VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(m_physicalDevice, &fbDepthFormat);
      assert(validDepthFormat);

      // Color attachment
      VkImageCreateInfo image = vks::initializers::imageCreateInfo();
      image.imageType = VK_IMAGE_TYPE_2D;
      image.format = FB_COLOR_FORMAT;
      image.extent.width = m_width;
      image.extent.height = m_height;
      image.extent.depth = 1;
      image.mipLevels = 1;
      image.arrayLayers = 1;
      image.samples = VK_SAMPLE_COUNT_1_BIT;
      image.tiling = VK_IMAGE_TILING_OPTIMAL;
      // We will sample directly from the color attachment
      image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

      VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
      VkMemoryRequirements memReqs;

      VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_colorAttachment.image));
      vkGetImageMemoryRequirements(m_device, m_colorAttachment.image, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_colorAttachment.mem));
      VK_CHECK_RESULT(vkBindImageMemory(m_device, m_colorAttachment.image, m_colorAttachment.mem, 0));

      VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
      colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      colorImageView.format = FB_COLOR_FORMAT;
      colorImageView.subresourceRange = {};
      colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      colorImageView.subresourceRange.baseMipLevel = 0;
      colorImageView.subresourceRange.levelCount = 1;
      colorImageView.subresourceRange.baseArrayLayer = 0;
      colorImageView.subresourceRange.layerCount = 1;
      colorImageView.image = m_colorAttachment.image;
      VK_CHECK_RESULT(vkCreateImageView(m_device, &colorImageView, nullptr, &m_colorAttachment.view));

      //// Create sampler to sample from the attachment in the fragment shader
      //VkSamplerCreateInfo samplerInfo = vks::initializers::samplerCreateInfo();
      //samplerInfo.magFilter = VK_FILTER_LINEAR;
      //samplerInfo.minFilter = VK_FILTER_LINEAR;
      //samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      //samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      //samplerInfo.addressModeV = samplerInfo.addressModeU;
      //samplerInfo.addressModeW = samplerInfo.addressModeU;
      //samplerInfo.mipLodBias = 0.0f;
      //samplerInfo.maxAnisotropy = 1.0f;
      //samplerInfo.minLod = 0.0f;
      //samplerInfo.maxLod = 1.0f;
      //samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      //VK_CHECK_RESULT(vkCreateSampler(device, &samplerInfo, nullptr, &offscreenPass.sampler));

      //// Depth stencil attachment
      //image.format = fbDepthFormat;
      //image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

      //VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &offscreenPass.depth.image));
      //vkGetImageMemoryRequirements(device, offscreenPass.depth.image, &memReqs);
      //memAlloc.allocationSize = memReqs.size;
      //memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      //VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &offscreenPass.depth.mem));
      //VK_CHECK_RESULT(vkBindImageMemory(device, offscreenPass.depth.image, offscreenPass.depth.mem, 0));

//      //// Create frame buffers for every swap chain image
//// frameBuffers.resize(swapChain.imageCount);
//      frameBuffers.resize(1);
//      for (uint32_t i = 0; i < frameBuffers.size(); i++)
//      {
//         // Color attachment
//         VkImageCreateInfo image = vks::initializers::imageCreateInfo();
//         image.imageType = VK_IMAGE_TYPE_2D;
//         image.format = FB_COLOR_FORMAT;
//         image.extent.width = offscreenPass.width;
//         image.extent.height = offscreenPass.height;
//         image.extent.depth = 1;
//         image.mipLevels = 1;
//         image.arrayLayers = 1;
//         image.samples = VK_SAMPLE_COUNT_1_BIT;
//         image.tiling = VK_IMAGE_TILING_OPTIMAL;
//         // We will sample directly from the color attachment
//         image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
//
//         VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
//         VkMemoryRequirements memReqs;
//
//         VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &m_imageRender[i]));
//         vkGetImageMemoryRequirements(device, m_imageRender[i], &memReqs);
//         memAlloc.allocationSize = memReqs.size;
//         memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//         VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &m_devicememoryRender[i]));
//         VK_CHECK_RESULT(vkBindImageMemory(device, m_imageRender[i], m_devicememoryRender[i], 0));
//
//
//      }
//      // Get the swap chain buffers containing the image and imageview
//      //buffers.resize(imageCount);
//      for (uint32_t i = 0; i < frameBuffers.size(); i++)
//      {
//         VkImageViewCreateInfo colorAttachmentView = {};
//         colorAttachmentView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
//         colorAttachmentView.pNext = NULL;
//         colorAttachmentView.format = FB_COLOR_FORMAT;
//         colorAttachmentView.components = {
//            VK_COMPONENT_SWIZZLE_R,
//            VK_COMPONENT_SWIZZLE_G,
//            VK_COMPONENT_SWIZZLE_B,
//            VK_COMPONENT_SWIZZLE_A
//         };
//         colorAttachmentView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
//         colorAttachmentView.subresourceRange.baseMipLevel = 0;
//         colorAttachmentView.subresourceRange.levelCount = 1;
//         colorAttachmentView.subresourceRange.baseArrayLayer = 0;
//         colorAttachmentView.subresourceRange.layerCount = 1;
//         colorAttachmentView.viewType = VK_IMAGE_VIEW_TYPE_2D;
//         colorAttachmentView.flags = 0;
//
//         colorAttachmentView.image = m_imageRender[i];
//
//         VK_CHECK_RESULT(vkCreateImageView(device, &colorAttachmentView, nullptr, &m_imageviewRender[i]));
//      }
//

      {
               // Depth stencil attachment
         image.format = m_depthFormat;
         image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

         VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_depthAttachment.image));
         vkGetImageMemoryRequirements(m_device, m_depthAttachment.image, &memReqs);
         memAlloc.allocationSize = memReqs.size;
         memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
         VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_depthAttachment.mem));
         VK_CHECK_RESULT(vkBindImageMemory(m_device, m_depthAttachment.image, m_depthAttachment.mem, 0));

         VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
         depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
         depthStencilView.format = m_depthFormat;
         depthStencilView.flags = 0;
         depthStencilView.subresourceRange = {};
         depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
         depthStencilView.subresourceRange.baseMipLevel = 0;
         depthStencilView.subresourceRange.levelCount = 1;
         depthStencilView.subresourceRange.baseArrayLayer = 0;
         depthStencilView.subresourceRange.layerCount = 1;
         depthStencilView.image = m_depthAttachment.image;
         VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_depthAttachment.view));
      }


      VkImageView attachments[2];

      // Depth/Stencil attachment is the same for all frame buffers
      attachments[0] = m_colorAttachment.view;
      attachments[1] = m_depthAttachment.view;

      VkFramebufferCreateInfo frameBufferCreateInfo = {};
      frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
      frameBufferCreateInfo.pNext = NULL;
      frameBufferCreateInfo.renderPass = m_renderPass;
      frameBufferCreateInfo.attachmentCount = 2;
      frameBufferCreateInfo.pAttachments = attachments;
      frameBufferCreateInfo.width = m_width;
      frameBufferCreateInfo.height = m_height;
      frameBufferCreateInfo.layers = 1;

      m_frameBuffers.resize(1);

      VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &frameBufferCreateInfo, nullptr, &m_frameBuffers[0]));


   }


   ::string VulkanExample5_base::getShadersPath() const
   {
      return "M:/Vulkan-Assets-main/shaders/glsl/"_ansi;
   }

   /** @brief Loads a SPIR-V shader file for the given shader stage */
   VkPipelineShaderStageCreateInfo VulkanExample5_base::loadShader(const ::string fileName, VkShaderStageFlagBits stage)
   {
      VkPipelineShaderStageCreateInfo shaderStage = {};
      shaderStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
      shaderStage.stage = stage;
#if defined(VK_USE_PLATFORM_ANDROID_KHR)
      shaderStage.module = vks::tools::loadShader(androidApp->activity->assetManager, fileName.c_str(), device);
#else
      shaderStage.module = vks::tools::loadShader(fileName.c_str(), m_device);
#endif
      shaderStage.pName = "main"_ansi;
      assert(shaderStage.module != VK_NULL_HANDLE);
      m_shaderModules.push_back(shaderStage.module);
      return shaderStage;
   }

   void VulkanExample5_base::_prepare()
   {
      //initSwapchain();
      //createCommandPool();
      //setupSwapChain();
      createTransferCommandPool();
      createCommandBuffers();
      createSynchronizationPrimitives();
      setupDepthStencil();
      setupRenderPass();
      createPipelineCache();
      setupFrameBuffer();
      m_settings.overlay = m_settings.overlay && (!m_benchmark.active);
      if (m_settings.overlay) {
         m_ui.device = m_pvulkandevice;
         m_ui.queue = m_queue;
         m_ui.shaders = {
            loadShader(getShadersPath() + "base/uioverlay.vert.spv"_ansi, VK_SHADER_STAGE_VERTEX_BIT),
            loadShader(getShadersPath() + "base/uioverlay.frag.spv"_ansi, VK_SHADER_STAGE_FRAGMENT_BIT),
         };
         m_ui.prepareResources();
         VkFormat colorFormat = VK_FORMAT_R8G8B8A8_UNORM;
         //ui.preparePipeline(pipelineCache, renderPass, swapChain.colorFormat, depthFormat);
         m_ui.preparePipeline(m_pipelineCache, m_renderPass, colorFormat, m_depthFormat);
      }
   }


   // Setup the offscreen framebuffer for rendering the mirrored scene
// The color attachment of this framebuffer will then be used to sample from in the fragment shader of the final pass
   void VulkanExample5_base::prepareOffscreen()
   {
      m_offscreenPass.width = FB_DIM;
      m_offscreenPass.height = FB_DIM;

      // Find a suitable depth format
      VkFormat fbDepthFormat;
      VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(m_physicalDevice, &fbDepthFormat);
      assert(validDepthFormat);

      // Color attachment
      VkImageCreateInfo image = vks::initializers::imageCreateInfo();
      image.imageType = VK_IMAGE_TYPE_2D;
      image.format = FB_COLOR_FORMAT;
      image.extent.width = m_offscreenPass.width;
      image.extent.height = m_offscreenPass.height;
      image.extent.depth = 1;
      image.mipLevels = 1;
      image.arrayLayers = 1;
      image.samples = VK_SAMPLE_COUNT_1_BIT;
      image.tiling = VK_IMAGE_TILING_OPTIMAL;
      // We will sample directly from the color attachment
      image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

      VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
      VkMemoryRequirements memReqs;

      VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_offscreenPass.color.image));
      vkGetImageMemoryRequirements(m_device, m_offscreenPass.color.image, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_offscreenPass.color.mem));
      VK_CHECK_RESULT(vkBindImageMemory(m_device, m_offscreenPass.color.image, m_offscreenPass.color.mem, 0));

      VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
      colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      colorImageView.format = FB_COLOR_FORMAT;
      colorImageView.subresourceRange = {};
      colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      colorImageView.subresourceRange.baseMipLevel = 0;
      colorImageView.subresourceRange.levelCount = 1;
      colorImageView.subresourceRange.baseArrayLayer = 0;
      colorImageView.subresourceRange.layerCount = 1;
      colorImageView.image = m_offscreenPass.color.image;
      VK_CHECK_RESULT(vkCreateImageView(m_device, &colorImageView, nullptr, &m_offscreenPass.color.view));

      // Create sampler to sample from the attachment in the fragment shader
      VkSamplerCreateInfo samplerInfo = vks::initializers::samplerCreateInfo();
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerInfo.addressModeV = samplerInfo.addressModeU;
      samplerInfo.addressModeW = samplerInfo.addressModeU;
      samplerInfo.mipLodBias = 0.0f;
      samplerInfo.maxAnisotropy = 1.0f;
      samplerInfo.minLod = 0.0f;
      samplerInfo.maxLod = 1.0f;
      samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      VK_CHECK_RESULT(vkCreateSampler(m_device, &samplerInfo, nullptr, &m_offscreenPass.sampler));

      // Depth stencil attachment
      image.format = fbDepthFormat;
      image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

      VK_CHECK_RESULT(vkCreateImage(m_device, &image, nullptr, &m_offscreenPass.depth.image));
      vkGetImageMemoryRequirements(m_device, m_offscreenPass.depth.image, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(m_device, &memAlloc, nullptr, &m_offscreenPass.depth.mem));
      VK_CHECK_RESULT(vkBindImageMemory(m_device, m_offscreenPass.depth.image, m_offscreenPass.depth.mem, 0));

      VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
      depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      depthStencilView.format = fbDepthFormat;
      depthStencilView.flags = 0;
      depthStencilView.subresourceRange = {};
      depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      if (fbDepthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
         depthStencilView.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
      depthStencilView.subresourceRange.baseMipLevel = 0;
      depthStencilView.subresourceRange.levelCount = 1;
      depthStencilView.subresourceRange.baseArrayLayer = 0;
      depthStencilView.subresourceRange.layerCount = 1;
      depthStencilView.image = m_offscreenPass.depth.image;
      VK_CHECK_RESULT(vkCreateImageView(m_device, &depthStencilView, nullptr, &m_offscreenPass.depth.view));

      // Create a separate render pass for the offscreen rendering as it may differ from the one used for scene rendering

      std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};
      // Color attachment
      attchmentDescriptions[0].format = FB_COLOR_FORMAT;
      attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
      attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      // Depth attachment
      attchmentDescriptions[1].format = fbDepthFormat;
      attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
      attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
      VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

      VkSubpassDescription subpassDescription = {};
      subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpassDescription.colorAttachmentCount = 1;
      subpassDescription.pColorAttachments = &colorReference;
      subpassDescription.pDepthStencilAttachment = &depthReference;

      // Use subpass dependencies for layout transitions
      std::array<VkSubpassDependency, 2> dependencies;

      dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
      dependencies[0].dstSubpass = 0;
      dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      dependencies[0].srcAccessMask = VK_ACCESS_NONE_KHR;
      dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

      dependencies[1].srcSubpass = 0;
      dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
      dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
      dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

      // Create the actual renderpass
      VkRenderPassCreateInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
      renderPassInfo.pAttachments = attchmentDescriptions.data();
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpassDescription;
      renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
      renderPassInfo.pDependencies = dependencies.data();

      VK_CHECK_RESULT(vkCreateRenderPass(m_device, &renderPassInfo, nullptr, &m_offscreenPass.renderPass));

      VkImageView attachments[2];
      attachments[0] = m_offscreenPass.color.view;
      attachments[1] = m_offscreenPass.depth.view;

      VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
      fbufCreateInfo.renderPass = m_offscreenPass.renderPass;
      fbufCreateInfo.attachmentCount = 2;
      fbufCreateInfo.pAttachments = attachments;
      fbufCreateInfo.width = m_offscreenPass.width;
      fbufCreateInfo.height = m_offscreenPass.height;
      fbufCreateInfo.layers = 1;

      VK_CHECK_RESULT(vkCreateFramebuffer(m_device, &fbufCreateInfo, nullptr, &m_offscreenPass.frameBuffer));

      // Fill a descriptor for later use in a descriptor set
      m_offscreenPass.descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      m_offscreenPass.descriptor.imageView = m_offscreenPass.color.view;
      m_offscreenPass.descriptor.sampler = m_offscreenPass.sampler;
   }


   void VulkanExample5_base::prepare()
   {
      _prepare();
      loadAssets();
      prepareOffscreen();
      prepareUniformBuffers();
      setupDescriptors();
      preparePipelines();
      buildCommandBuffers();
      m_prepared = true;
   }


   void VulkanExample5_base::preparePipelines()
   {
      // Layouts
      VkPipelineLayoutCreateInfo pipelineLayoutInfo = vks::initializers::pipelineLayoutCreateInfo(&m_descriptorSetLayouts.shaded, 1);
      VK_CHECK_RESULT(vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayouts.shaded));

      pipelineLayoutInfo = vks::initializers::pipelineLayoutCreateInfo(&m_descriptorSetLayouts.textured, 1);
      VK_CHECK_RESULT(vkCreatePipelineLayout(m_device, &pipelineLayoutInfo, nullptr, &m_pipelineLayouts.textured));

      // Pipelines
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
      VkPipelineRasterizationStateCreateInfo rasterizationState = vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);
      VkPipelineColorBlendAttachmentState blendAttachmentState = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
      VkPipelineColorBlendStateCreateInfo colorBlendState = vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
      VkPipelineDepthStencilStateCreateInfo depthStencilState = vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
      VkPipelineViewportStateCreateInfo viewportState = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);
      VkPipelineMultisampleStateCreateInfo multisampleState = vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, 0);
      std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
      VkPipelineDynamicStateCreateInfo dynamicState = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);
      std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

      VkGraphicsPipelineCreateInfo pipelineCI = vks::initializers::pipelineCreateInfo(m_pipelineLayouts.textured, m_renderPass, 0);
      pipelineCI.pInputAssemblyState = &inputAssemblyState;
      pipelineCI.pRasterizationState = &rasterizationState;
      pipelineCI.pColorBlendState = &colorBlendState;
      pipelineCI.pMultisampleState = &multisampleState;
      pipelineCI.pViewportState = &viewportState;
      pipelineCI.pDepthStencilState = &depthStencilState;
      pipelineCI.pDynamicState = &dynamicState;
      pipelineCI.stageCount = static_cast<uint32_t>(shaderStages.size());
      pipelineCI.pStages = shaderStages.data();
      pipelineCI.pVertexInputState = vkglTF::Vertex::getPipelineVertexInputState({ vkglTF::VertexComponent::Position, vkglTF::VertexComponent::Color, vkglTF::VertexComponent::Normal });

      rasterizationState.cullMode = VK_CULL_MODE_NONE;

      // Render-target debug display
      shaderStages[0] = loadShader(getShadersPath() + "offscreen/quad.vert.spv"_ansi, VK_SHADER_STAGE_VERTEX_BIT);
      shaderStages[1] = loadShader(getShadersPath() + "offscreen/quad.frag.spv"_ansi, VK_SHADER_STAGE_FRAGMENT_BIT);
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_device, m_pipelineCache, 1, &pipelineCI, nullptr, &m_pipelines.debug));

      // Mirror
      shaderStages[0] = loadShader(getShadersPath() + "offscreen/mirror.vert.spv"_ansi, VK_SHADER_STAGE_VERTEX_BIT);
      shaderStages[1] = loadShader(getShadersPath() + "offscreen/mirror.frag.spv"_ansi, VK_SHADER_STAGE_FRAGMENT_BIT);
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_device, m_pipelineCache, 1, &pipelineCI, nullptr, &m_pipelines.mirror));

      rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;

      // Phong shading pipelines
      pipelineCI.layout = m_pipelineLayouts.shaded;
      // Scene
      shaderStages[0] = loadShader(getShadersPath() + "offscreen/phong.vert.spv"_ansi, VK_SHADER_STAGE_VERTEX_BIT);
      shaderStages[1] = loadShader(getShadersPath() + "offscreen/phong.frag.spv"_ansi, VK_SHADER_STAGE_FRAGMENT_BIT);
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_device, m_pipelineCache, 1, &pipelineCI, nullptr, &m_pipelines.shaded));
      // Offscreen
      // Flip cull mode
      rasterizationState.cullMode = VK_CULL_MODE_FRONT_BIT;
      pipelineCI.renderPass = m_offscreenPass.renderPass;
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(m_device, m_pipelineCache, 1, &pipelineCI, nullptr, &m_pipelines.shadedOffscreen));

   }

   // Prepare and initialize uniform buffer containing shader uniforms
   void VulkanExample5_base::prepareUniformBuffers()
   {
      // Mesh vertex shader uniform buffer block
      VK_CHECK_RESULT(m_pvulkandevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &m_uniformBuffers.vsShared, sizeof(UniformData)));
      // Mirror plane vertex shader uniform buffer block
      VK_CHECK_RESULT(m_pvulkandevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &m_uniformBuffers.vsMirror, sizeof(UniformData)));
      // Offscreen vertex shader uniform buffer block
      VK_CHECK_RESULT(m_pvulkandevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &m_uniformBuffers.vsOffScreen, sizeof(UniformData)));
      // Map persistent
      VK_CHECK_RESULT(m_uniformBuffers.vsShared.map());
      VK_CHECK_RESULT(m_uniformBuffers.vsMirror.map());
      VK_CHECK_RESULT(m_uniformBuffers.vsOffScreen.map());

      updateUniformBuffers();
      updateUniformBufferOffscreen();
   }

   void VulkanExample5_base::updateUniformBuffers()
   {
      m_uniformData.projection = m_camera.matrices.perspective;
      m_uniformData.view = m_camera.matrices.view;

      // Model
      m_uniformData.model = glm::mat4(1.0f);
      m_uniformData.model = glm::rotate(m_uniformData.model, glm::radians(m_modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      m_uniformData.model = glm::translate(m_uniformData.model, m_modelPosition);
      memcpy(m_uniformBuffers.vsShared.mapped, &m_uniformData, sizeof(UniformData));

      // Mirror
      m_uniformData.model = glm::mat4(1.0f);
      memcpy(m_uniformBuffers.vsMirror.mapped, &m_uniformData, sizeof(UniformData));
   }

   void VulkanExample5_base::updateUniformBufferOffscreen()
   {
      m_uniformData.projection = m_camera.matrices.perspective;
      m_uniformData.view = m_camera.matrices.view;
      m_uniformData.model = glm::mat4(1.0f);
      m_uniformData.model = glm::rotate(m_uniformData.model, glm::radians(m_modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      m_uniformData.model = glm::scale(m_uniformData.model, glm::vec3(1.0f, -1.0f, 1.0f));
      m_uniformData.model = glm::translate(m_uniformData.model, m_modelPosition);
      memcpy(m_uniformBuffers.vsOffScreen.mapped, &m_uniformData, sizeof(UniformData));
   }


   //void prepareFrame()
   //{
   //   // Acquire the next image from the swap chain
   //   VkResult result = m_swapchain.acquireNextImage(m_semaphores.presentComplete, &m_currentBuffer);
   //   // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE)
   //   // SRS - If no longer optimal (VK_SUBOPTIMAL_KHR), wait until submitFrame() in case number of swapchain images will change on resize
   //   if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
   //      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
   //         //windowResize();
   //      }
   //      return;
   //   }
   //   else {
   //      VK_CHECK_RESULT(result);
   //   }
   //}

   //void submitFrame()
   //{
   //   VkResult result = m_swapchain.queuePresent(queue, m_currentBuffer, m_semaphores.renderComplete);
   //   // Recreate the swapchain if it's no longer compatible with the surface (OUT_OF_DATE) or no longer optimal for presentation (SUBOPTIMAL)
   //   if ((result == VK_ERROR_OUT_OF_DATE_KHR) || (result == VK_SUBOPTIMAL_KHR)) {
   //      //windowResize();
   //      if (result == VK_ERROR_OUT_OF_DATE_KHR) {
   //         return;
   //      }
   //   }
   //   else {
   //      VK_CHECK_RESULT(result);
   //   }
   //   VK_CHECK_RESULT(vkQueueWaitIdle(queue));
   //}


   void VulkanExample5_base::draw(const ::function < void(void *, int, int, int)> & callback)
   {
      //prepareFrame();
      //m_submitInfo.commandBufferCount = 1;
      //m_submitInfo.pCommandBuffers = &m_drawCmdBuffers[m_currentBuffer];
      submitWork(m_drawCmdBuffers[m_currentBuffer], m_queue);
      vkDeviceWaitIdle(m_device);
      //VK_CHECK_RESULT(vkQueueSubmit(m_queue, 1, &m_submitInfo, VK_NULL_HANDLE));
      sample(callback);
      //submitFrame();
   }

   void VulkanExample5_base::render(const ::function < void(void *, int, int, int)> & callback)
   {
      if (!m_prepared)
         return;
      draw(callback);
      if (!m_paused || m_camera.updated)
      {
         if (!m_paused) {
            m_modelRotation.y += m_frameTimer * 10.0f;
         }
         updateUniformBuffers();
         updateUniformBufferOffscreen();
      }
   }



#if defined(VK_USE_PLATFORM_ANDROID_KHR)
void handleAppCommand(android_app * app, int32_t cmd) {
   if (cmd == APP_CMD_INIT_WINDOW) {
      VulkanExample5 * vulkanExample = ___new VulkanExample5();
      delete(vulkanExample);
      ANativeActivity_finish(app->activity);
   }
}
void android_main(android_app * state) {
   androidapp = state;
   androidapp->onAppCmd = handleAppCommand;
   int ident, happenings;
   struct android_poll_source * source;
   while ((ident = ALooper_pollAll(-1, NULL, &happenings, (void **)&source)) >= 0) {
      if (source != NULL) {
         source->process(androidapp, source);
      }
      if (androidapp->destroyRequested != 0) {
         break;
      }
   }
}
#else
int run_vulkan_example5(mouseState * pmousestate, const ::function < void(void *, int, int, int)> & callback) 
{
   VulkanExample5 * vulkanExample = ___new VulkanExample5(pmousestate);
   vulkanExample->initVulkan();

   vulkanExample->prepare();
   vulkanExample->renderLoop2(callback);
  
//   std::cout << "Finished. Press enter to terminate..."_ansi;
  // getchar();
   delete vulkanExample;
   return 0;
}
#endif