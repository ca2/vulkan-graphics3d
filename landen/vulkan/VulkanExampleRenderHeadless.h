#pragma once


/*
* Vulkan Example - Minimal headless rendering example
*
* Copyright (C) 2017 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "framework.h"
#include "acme/_operating_system.h"
#include "VulkanTools.h"
#include "benchmark.hpp"
#include "camera.hpp"
//#include "VulkanSwapChain.h"
#include "VulkanglTFModel.h"
#include "ui_overlay.h"
#include "shared.h"
//#if defined(_WIN32)
//#pragma comment(linker, "/subsystem:console"_ansi)
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//#include <android/native_activity.h>
//#include <android/asset_manager.h>
//#include <android_native_app_glue.h>
//#include <android/log.h>
//#include "VulkanAndroid.h"
//#endif

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <assert.h>
//#include <vector>
//#include <array>
//#include <iostream>
//#include <algorithm>
//
//#define GLM_FORCE_RADIANS
//#define GLM_FORCE_DEPTH_ZERO_TO_ONE
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//
//#include <vulkan/vulkan.h>
//#include "VulkanTools.h"
//
//#if defined(VK_USE_PLATFORM_ANDROID_KHR)
//android_app * androidapp;
//#endif
//
////#define DEBUG (!NDEBUG)
//
//#define BUFFER_ELEMENTS 32
//
//#if defined(VK_USE_PLATFORM_ANDROID_KHR)
//#define LOG(...) ((void)__android_log_print(ANDROID_LOG_INFO, "vulkanExample"_ansi, __VA_ARGS__))
//#else
//#define LOG(...) printf(__VA_ARGS__)
//#endif
//
//static VKAPI_ATTR VkBool32 VKAPI_CALL debugMessageCallback(
//   VkDebugReportFlagsEXT flags,
//   VkDebugReportObjectTypeEXT objectType,
//   uint64_t object,
//   size_t location,
//   int32_t messageCode,
//   const char * pLayerPrefix,
//   const char * pMessage,
//   void * pUserData)
//{
//   LOG("[VALIDATION]: %s - %s\n"_ansi, pLayerPrefix, pMessage);
//   return VK_FALSE;
//}
//


class VulkanExampleRenderHeadless
{
public:

   mouseState * m_pmousestate;

   bool m_debugDisplay = false;

   VkClearColorValue m_defaultClearColor = { { 0.325f, 0.325f, 0.325f, 1.0f } };

   std::vector<std::string> m_supportedInstanceExtensions;
   std::vector<const char *> m_enabledDeviceExtensions;
   std::vector<const char *> m_enabledInstanceExtensions;
   vks::Benchmark m_benchmark;

   Camera m_camera;

   bool m_requiresStencil{ false };
   // Frame counter to display fps
   uint32_t m_frameCounter = 0;
   uint32_t m_lastFPS = 0;
   std::chrono::time_point<std::chrono::high_resolution_clock> m_lastTimestamp, m_tPrevEnd;

   ::string m_strApplicationName;
   ::string m_strEngineName;
   uint32_t m_apiVersion = VK_API_VERSION_1_0;


   struct {
      vulkan::buffer vsShared;
      vulkan::buffer vsMirror;
      vulkan::buffer vsOffScreen;
   } m_uniformBuffers;

   struct UniformData {
      glm::mat4 projection;
      glm::mat4 view;
      glm::mat4 model;
      glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
   } m_uniformData;

   struct {
      VkPipeline debug{ VK_NULL_HANDLE };
      VkPipeline shaded{ VK_NULL_HANDLE };
      VkPipeline shadedOffscreen{ VK_NULL_HANDLE };
      VkPipeline mirror{ VK_NULL_HANDLE };
   } m_pipelines;

   struct {
      VkPipelineLayout textured{ VK_NULL_HANDLE };
      VkPipelineLayout shaded{ VK_NULL_HANDLE };
   } m_pipelineLayouts;

   struct {
      VkDescriptorSet offscreen{ VK_NULL_HANDLE };
      VkDescriptorSet mirror{ VK_NULL_HANDLE };
      VkDescriptorSet model{ VK_NULL_HANDLE };
   } m_descriptorSets;


   struct {
      VkDescriptorSetLayout textured{ VK_NULL_HANDLE };
      VkDescriptorSetLayout shaded{ VK_NULL_HANDLE };
   } m_descriptorSetLayouts;

   //VulkanSwapChain m_swapchain;
   //// Synchronization semaphores
   //struct {
   //   // Swap chain image presentation
   //   VkSemaphore presentComplete;
   //   // Command buffer submission and execution
   //   VkSemaphore renderComplete;
   //} m_semaphores;


   /** @brief Default depth stencil attachment used by the default render pass */
   struct {
      VkImage image;
      VkDeviceMemory memory;
      VkImageView view;
   } m_depthStencil{};

   std::vector<VkFence> m_waitFences;
   // Physical device (GPU) that Vulkan will use
   VkPhysicalDevice m_hysicalDevice{ VK_NULL_HANDLE };
   // Stores physical device properties (for e.g. checking device limits)
   VkPhysicalDeviceProperties m_deviceProperties{};
   // Stores the features available on the selected physical device (for e.g. checking if a feature is available)
   VkPhysicalDeviceFeatures m_deviceFeatures{};
   // Stores all available memory (type) properties for the physical device
   VkPhysicalDeviceMemoryProperties m_deviceMemoryProperties{};
   /** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
   VkPhysicalDeviceFeatures m_enabledFeatures{};
   /** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
   /** @brief Optional pNext structure for passing extension structures to device creation */
   void * m_deviceCreatepNextChain = nullptr;
   /** @brief Logical device, application's view of the physical device (GPU) */
   VkDevice m_device{ VK_NULL_HANDLE };
   // Handle to the device graphics queue that command buffers are submitted to
   VkQueue m_queue{ VK_NULL_HANDLE };
   // Depth buffer format (selected during Vulkan initialization)
   VkFormat m_depthFormat;
   // Command buffer pool
   //VkCommandPool cmdPool{ VK_NULL_HANDLE };
   /** @brief Pipeline stages used to wait at for graphics queue submissions */
   VkPipelineStageFlags m_submitPipelineStages = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
   // Contains command buffers and semaphores to be presented to the queue
   //VkSubmitInfo m_submitInfo;

   // Active frame buffer index
   uint32_t m_currentBuffer = 0;
   // Descriptor set pool
   VkDescriptorPool m_descriptorPool{ VK_NULL_HANDLE };
   //struct {
   //   VkDescriptorSetLayout textured{ VK_NULL_HANDLE };
   //   VkDescriptorSetLayout shaded{ VK_NULL_HANDLE };
   //} m_descriptorSetLayouts;


   // Contains command buffers and semaphores to be presented to the queue
   //VkSubmitInfo m_submitInfo;
   // Command buffers used for rendering
   std::vector<VkCommandBuffer> m_drawCmdBuffers;
   // Global render pass for frame buffer writes
   // VkRenderPass renderPass{ VK_NULL_HANDLE };
   // List of available frame buffers (same as number of swap chain images)
   std::vector<VkFramebuffer>m_frameBuffers;
   bool m_prepared = false;
   bool m_resized = false;
   bool m_viewUpdated = false;
   bool m_paused = false;
   // Defines a frame rate independent timer value clamped from -1.0...1.0
// For use in animations, rotations, etc.
   float m_fTimer = 0.0f;
   // Multiplier for speeding up (or slowing down) the global timer
   float m_fTimerSpeed = 0.25f;
   uint32_t m_width = 1280;
   uint32_t m_height = 720;
   glm::vec3 m_modelPosition = glm::vec3(0.0f, -1.0f, 0.0f);
   glm::vec3 m_modelRotation = glm::vec3(0.0f);
   float m_frameTimer = 1.0f;

   // Framebuffer for offscreen rendering
   struct FrameBufferAttachment {
      VkImage image;
      VkDeviceMemory mem;
      VkImageView view;
   };
   /** @brief Encapsulated physical and logical vulkan device */
   ::vulkan::device * m_pvulkandevice = nullptr;

   /** @brief Example settings that can be changed e.g. by command line arguments */
   struct Settings {
      /** @brief Activates validation layers (and message output) when set to true */
      bool validation = false;
      /** @brief Set to true if fullscreen mode has been requested via command line */
      bool fullscreen = false;
      /** @brief Set to true if v-sync will be forced for the swapchain */
      bool vsync = false;
      /** @brief Enable UI overlay */
      bool overlay = true;
   } m_settings;


   /** @brief State of gamepad input (only used on Android) */
   struct {
      glm::vec2 axisLeft = glm::vec2(0.0f);
      glm::vec2 axisRight = glm::vec2(0.0f);
   } m_gamePadState;

   vulkan::ui_overlay m_ui;


   struct OffscreenPass {
      int32_t width, height;
      VkFramebuffer frameBuffer;
      FrameBufferAttachment color, depth;
      VkRenderPass renderPass;
      VkSampler sampler;
      VkDescriptorImageInfo descriptor;
   } m_offscreenPass{};


   VkInstance m_instance;
   VkPhysicalDevice m_physicalDevice;
   //VkDevice device;
   uint32_t m_queueFamilyIndex;
   VkPipelineCache m_pipelineCache;
   //VkQueue queue;
   VkCommandPool m_commandPoolTransfer;
   VkCommandBuffer m_commandBuffer;
   VkDescriptorSetLayout m_descriptorSetLayout;
   VkPipelineLayout m_pipelineLayout;
   VkPipeline m_pipeline;
   std::vector<VkShaderModule> m_shaderModules;
   VkBuffer m_vertexBuffer, m_indexBuffer;
   VkDeviceMemory m_vertexMemory, m_indexMemory;

   //int32_t width, height;
   VkFramebuffer m_framebuffer;
   FrameBufferAttachment m_colorAttachment, m_depthAttachment;
   VkRenderPass m_renderPass;

   VkDebugReportCallbackEXT m_debugReportCallback{};

   VulkanExampleRenderHeadless(mouseState * pmousestate);
   ~VulkanExampleRenderHeadless();


   VkResult createInstance();

   void getEnabledFeatures();

   void getEnabledExtensions();

   bool initVulkan();
   void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);



   void renderLoop2(const ::function < void(void *, int, int, int)> & callback);

   void sample(const ::function < void(void *, int, int, int)> & callback);

   void drawUI(const VkCommandBuffer commandBuffer);
   void createTransferCommandPool();

   virtual void buildCommandBuffers();


   void nextFrame(const ::function < void(void *, int, int, int)> & callback);


   void OnUpdateUIOverlay(vulkan::ui_overlay * overlay);

   void updateOverlay();


   virtual void loadAssets();

   void setupDescriptors();


   void createCommandBuffers();

   void createSynchronizationPrimitives();

   void setupDepthStencil();

   void setupRenderPass();

   void createPipelineCache();

   //VkImage m_imageRender[1];
   //VkDeviceMemory m_devicememoryRender[1];
   //VkImageView m_imageviewRender[1];

   void setupFrameBuffer();

   ::string getShadersPath() const;

   /** @brief Loads a SPIR-V shader file for the given shader stage */
   VkPipelineShaderStageCreateInfo loadShader(const ::string fileName, VkShaderStageFlagBits stage);

   void _prepare();


   void prepareOffscreen();

   void prepare();


   void preparePipelines();

   // Prepare and initialize uniform buffer containing shader uniforms
   void prepareUniformBuffers();

   void updateUniformBuffers();

   void updateUniformBufferOffscreen();



   void draw(const ::function < void(void *, int, int, int)> & callback);

   void render(const ::function < void(void *, int, int, int)> & callback);


};


class VulkanExample5 :
   public VulkanExampleRenderHeadless
{
public:

   struct {
      vkglTF::Model example;
      vkglTF::Model plane;
   } m_models;

   VulkanExample5(mouseState * pmousestate);
   ~VulkanExample5();

   void loadAssets()override;

   void buildCommandBuffers() override;

};
