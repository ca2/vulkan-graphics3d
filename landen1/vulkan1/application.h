// Created by camilo on 2024-07-27 <3ThomasBorregaardSorensen!!
#pragma once


#include "_vulkan.h"
#include "vulkan-land/landen/vulkan/shared.h"

#include <chrono>


#include "keycodes.h"
#include "tools.h"
#include "debug.h"
#include "ui_overlay.h"
#include "swap_chain.h"
#include "buffer.h"
#include "device.h"
#include "texture.h"


#include "initializers.h"
#include "camera.h"
#include "benchmark.h"
#include "application.h"




namespace vulkan1
{


   class CLASS_DECL_VULKAN_LAND_LANDEN application :
      virtual public ::particle
   {
   public:


      // Frame counter to display fps
      uint32_t m_uFrameCounter;
      uint32_t m_uLastFPS;
      std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp, tPrevEnd;
      // Vulkan instance, stores all per-application states
      //VkInstance instance{ VK_NULL_HANDLE };
      //std::vector<std::string> supportedInstanceExtensions;
      // Physical device (GPU) that Vulkan will use
      VkPhysicalDevice m_vkphysicaldevice;
      // Stores physical device properties (for e.g. checking device limits)
      VkPhysicalDeviceProperties m_vkphysicaldeviceproperties;
      // Stores the features available on the selected physical device (for e.g. checking if a feature is available)
      VkPhysicalDeviceFeatures m_vkphysicaldevicefeatures;
      // Stores all available memory (type) properties for the physical device
      VkPhysicalDeviceMemoryProperties m_vkphysicaldevicememoryproperties;
      /// @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
      VkPhysicalDeviceFeatures m_vkphysicaldevicefeaturesEnabled;
      /// @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
      //std::vector<const char*> m_pszaEnabledDeviceExtensions;
      //std::vector<const char*> m_pszaEnabledInstanceExtensions;
      /// @brief Optional pNext structure for passing extension structures to device creation */
      void* m_pDeviceCreateNextChain;
      /// @brief Logical device, application's view of the physical device (GPU) */
      VkDevice m_vkdevice;
      // Handle to the device graphics m_vkqueue that command buffers are submitted to
      VkQueue m_vkqueue;
      // Depth buffer format (selected during Vulkan initialization)
      VkFormat m_vkformatDepth;
      // Command buffer pool
      VkCommandPool m_vkcommandpool;
      /// @brief Pipeline stages used to wait at for graphics m_vkqueue submissions */
      VkPipelineStageFlags m_vkpipelinestageflagsSubmit;
      // Contains command buffers and semaphores to be presented to the m_vkqueue
      VkSubmitInfo m_vksubmitinfo;
      std::vector<VkCommandBuffer> m_vkcommandbuffersDraw;
      // Command buffers used for rendering
      std::vector<VkCommandBuffer> m_vkcommandbuffersStencil;
      // Global render pass for frame buffer writes
      VkRenderPass m_vkrenderpass;
      // List of available frame buffers (same as number of swap chain images)
      std::vector<VkFramebuffer>m_vkframebuffers;
      // Active frame buffer index
      uint32_t m_uiCurrentBuffer;
      // Descriptor set pool
      VkDescriptorPool m_vkdescriptorpool;
      // List of shader modules created (stored for cleanup)
      std::vector<VkShaderModule> m_vkshadermodules;
      // Pipeline cache object
      VkPipelineCache m_vkpipelinecache;
      // Wraps the swap chain to present images (framebuffers) to the windowing system
      vulkan::swap_chain m_swapchain;
      // Synchronization semaphores
      struct {
         // Swap chain image presentation
         VkSemaphore m_vksemaphorePresentComplete;
         // Command buffer submission and execution
         VkSemaphore m_vksemaphoreRenderComplete;
      } m_semaphores;
      std::vector<VkFence> m_vkfencesWait;
      bool m_bRequiresStencil;
   ///public:
      bool m_bPrepared;
      bool m_bResized;
      bool m_bViewUpdated;
      int m_iWidth;
      int m_iHeight;

      vulkan::ui_overlay m_uioverlay;
      //CommandLineParser commandLineParser;

      /// @brief Last frame time measured using a high performance m_fTimer (if available) */
      float m_fFrameTimer;

      vks::Benchmark m_benchmark;

      /// @brief Encapsulated physical and logical vulkan device */
      ::vulkan::device* m_pvulkandevice = nullptr;

      ///// @brief Example m_settings that can be changed e.g. by command line arguments */
      //struct Settings {
      //   /// @brief Activates validation layers (and message output) when set to true */
      //   bool validation = false;
      //   /// @brief Set to true if m_bFullScreen mode has been requested via command line */
      //   bool m_bFullScreen = false;
      //   /// @brief Set to true if v-sync will be forced for the swapchain */
      //   bool vsync = false;
      //   /// @brief Enable UI overlay */
      //   bool overlay = true;
      //} m_settings;

      /// @brief State of gamepad input (only used on Android) */
      struct {
         glm::vec2 axisLeft = glm::vec2(0.0f);
         glm::vec2 axisRight = glm::vec2(0.0f);
      } m_gamepadstate;

      ///// @brief State of mouse/touch input */
      //struct {
      //   struct {
      //      bool left = false;
      //      bool right = false;
      //      bool middle = false;
      //   } m_buttons;
      //   glm::vec2 position;
      //} m_pmousestate;

      mouse_state* m_pmousestate;
      uint32_t          m_uQueueFamilyIndex;
      VkFormat          m_vkformatColorFB;
      VkClearColorValue m_vkclearcolorvalueDefault;

      ::string_array       m_strArgs;

      // Defines a frame rate independent m_fTimer value clamped from -1.0...1.0
      // For use in animations, rotations, etc.
      float m_fTimer;
      // Multiplier for speeding up (or slowing down) the global m_fTimer
      float m_fTimerSpeed;
      bool m_bPaused;

      Camera m_camera;

      //::string m_strTitle = "Vulkan Example"_ansi;
      //::string m_strWindowClassName = "vulkanExample"_ansi;
      // uint32_t apiVersion = VK_API_VERSION_1_0;

      /// @brief Default depth stencil attachment used by the default render pass */
      struct depth_stencil {
         VkImage image;
         VkDeviceMemory memory;
         VkImageView view;
      } m_depthstencil;



      VkInstance        m_vkinstance;
      comparable_array < const char* >    m_pszaSupportedInstanceExtensions;
      comparable_array < const char* >    m_pszaEnabledDeviceExtensions;
      comparable_array < const char* >    m_pszaEnabledInstanceExtensions;
      ::string          m_strTitle;
      ::string          m_strWindowClassName;
      uint32_t          m_uApiVersion;
      ::string          m_strShaderDir;


      uint32_t m_iDestWidth;
      uint32_t m_iDestHeight;
      bool m_bResizing;
      bool m_bQuit;
      bool m_bConfigured;
      struct FrameBufferAttachment {
      VkImage image;
      VkDeviceMemory mem;
      VkImageView view;
   };
   FrameBufferAttachment m_attachmentColor;


      /// @brief Example m_settings that can be changed e.g. by command line arguments */
      struct m_settings 
      {
         /// @brief Activates validation layers (and message output) when set to true */
         bool m_bValidation;
         /// @brief Set to true if m_bFullScreen mode has been requested via command line */
         bool m_bFullScreen;
         /// @brief Set to true if v-sync will be forced for the swapchain */
         bool m_bVSync;
         /// @brief Enable UI overlay */
         bool m_bUIOverlay;

         m_settings();

      } m_settings;


      ::function < void(void*, int, int, int)> m_callbackOffscreen;


      application();
      ~application() override;



      virtual ::comparable_array < const char * > get_instance_extensions();

      virtual VkResult createInstance();


      virtual void handleMouseMove(int x, int y);

      virtual void run_application();

      virtual void resize(int cx, int cy);
      /** @brief Setup the vulkan instance, enable required extensions and connect to the physical device (GPU) */
      virtual bool initVulkan();


      std::string getWindowTitle();

      std::string getShadersPath() const;


      //virtual void handleMouseMove(int32_t x, int32_t y);
      virtual void nextFrame(const ::function < void(void*, int, int, int)>& callback);
      virtual void updateOverlay();
      virtual void createPipelineCache();
      virtual void createCommandPool();
      virtual void createSynchronizationPrimitives();
      virtual void initSwapchain();
      virtual void setupSwapChain();
      virtual void createCommandBuffers();
      virtual void destroyCommandBuffers();
      /** @brief (Virtual) Creates the application wide Vulkan instance */
      //virtual VkResult createInstance();
      /** @brief (Pure virtual) Render function to be implemented by the sample application */
      virtual void render(const ::function < void(void*, int, int, int)>& callback);
      /** @brief (Virtual) Called after a key was pressed, can be used to do custom key handling */
      virtual void keyPressed(uint32_t);
      /** @brief (Virtual) Called after the mouse cursor moved and before internal happenings (like m_camera rotation) is handled */
      virtual void mouseMoved(double x, double y, bool& handled);
      /** @brief (Virtual) Called when the window has been m_bResized, can be used by the sample application to recreate resources */
      virtual void windowResized();
      /** @brief (Virtual) Called when resources have been recreated that require a rebuild of the command buffers (e.g. frame buffer), to be implemented by the sample application */
      virtual void buildCommandBuffers();
      /** @brief (Virtual) Setup default depth and stencil views */
      virtual void setupDepthStencil();
      /** @brief (Virtual) Setup default framebuffers for all requested swapchain images */
      virtual void setupFrameBuffer();
      /** @brief (Virtual) Setup a default renderpass */
      virtual void setupRenderPass();
      /** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
      virtual void getEnabledFeatures();
      /** @brief (Virtual) Called after the physical device extensions have been read, can be used to enable extensions based on the supported extension listing*/
      virtual void getEnabledExtensions();

      /** @brief Prepares all Vulkan resources and functions required to run the sample */
      virtual void prepare();

      /** @brief Loads a SPIR-V shader file for the given shader stage */
      VkPipelineShaderStageCreateInfo loadShader(std::string fileName, VkShaderStageFlagBits stage);

      void windowResize();

      /** @brief Entry point for the main render loop */
      void renderLoop();

      /** @brief Adds the drawing commands for the ImGui overlay to the given command buffer */
      void drawUI(const VkCommandBuffer commandBuffer);

      /** Prepare the next frame for workload submission by acquiring the next swap chain image */
      void prepareFrame();
      /** @brief Presents the current image to the swap chain */
      void submitFrame();
      /** @brief (Virtual) Default image acquire + submission and command buffer submission function */
      virtual void renderFrame();

      /** @brief (Virtual) Called when the UI overlay is updating, can be used to add custom elements to the overlay */
      virtual void OnUpdateUIOverlay(vulkan::ui_overlay* overlay);


   };



} // namespace vulkan1


