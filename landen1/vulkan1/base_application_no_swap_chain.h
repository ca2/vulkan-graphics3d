/*
* Vulkan Example base class
*
* Copyright (C) 2016-2024 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/

#pragma once


#include "application.h"

#include "shared.h"
// #ifdef _WIN32
// #pragma comment(linker, "/subsystem:windows")
// #include <windows.h>
// #include <fcntl.h>
// #include <io.h>
// #include <ShellScalingAPI.h>
// #elif defined(VK_USE_PLATFORM_ANDROID_KHR)
// #include <android/native_activity.h>
// #include <android/asset_manager.h>
// #include <android_native_app_glue.h>
// #include <sys/system_properties.h>
// #include "VulkanAndroid.h"
// #elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
// #include <directfb.h>
// #elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
// #include <wayland-client.h>
// #include "xdg-shell-client-protocol.h"
// #elif defined(_DIRECT2DISPLAY)
// //
// #elif defined(VK_USE_PLATFORM_XCB_KHR)
// #include <xcb/xcb.h>
// #endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <array>
#include <unordered_map>
#include <numeric>
#include <ctime>
#include <iostream>
#include <random>
#include <algorithm>
#include <sys/stat.h>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <numeric>
#include <array>


//#include "CommandLineParser.hpp"



namespace vulkan1
{


   class CLASS_DECL_VULKAN_LAND_LANDEN base_application_no_swap_chain :
      virtual public ::vulkan::application
   {
   public:

   //   std::string shaderDir = "glsl";
   //   //protected:
   //      // Returns the path to the root of the glsl or hlsl shader directory.

   //      // Frame counter to display fps
   //   uint32_t m_uFrameCounter = 0;
   //   uint32_t m_uLastFPS = 0;
   //   std::chrono::time_point<std::chrono::high_resolution_clock> lastTimestamp, tPrevEnd;
   //   // Vulkan instance, stores all per-application states
   //   //VkInstance instance{ VK_NULL_HANDLE };
   //   //std::vector<std::string> supportedInstanceExtensions;
   //   // Physical device (GPU) that Vulkan will use
   //   VkPhysicalDevice m_vkphysicaldevice{ VK_NULL_HANDLE };
   //   // Stores physical device properties (for e.g. checking device limits)
   //   VkPhysicalDeviceProperties m_vkphysicaldeviceproperties{};
   //   // Stores the features available on the selected physical device (for e.g. checking if a feature is available)
   //   VkPhysicalDeviceFeatures m_vkphysicaldevicefeatures{};
   //   // Stores all available memory (type) properties for the physical device
   //   VkPhysicalDeviceMemoryProperties m_vkphysicaldevicememoryproperties{};
   //   /** @brief Set of physical device features to be enabled for this example (must be set in the derived constructor) */
   //   VkPhysicalDeviceFeatures m_vkphysicaldevicefeaturesEnabled{};
   //   /** @brief Set of device extensions to be enabled for this example (must be set in the derived constructor) */
   //   //std::vector<const char*> m_pszaEnabledDeviceExtensions;
   //   //std::vector<const char*> m_pszaEnabledInstanceExtensions;
   //   /** @brief Optional pNext structure for passing extension structures to device creation */
   //   void* m_pDeviceCreateNextChain = nullptr;
   //   /** @brief Logical device, application's view of the physical device (GPU) */
   //   VkDevice device{ VK_NULL_HANDLE };
   //   // Handle to the device graphics m_vkqueue that command buffers are submitted to
   //   VkQueue m_vkqueue{ VK_NULL_HANDLE };
   //   // Depth buffer format (selected during Vulkan initialization)
   //   VkFormat m_vkformatDepth;
   //   // Command buffer pool
   //   VkCommandPool m_vkcommandpool{ VK_NULL_HANDLE };
   //   /** @brief Pipeline stages used to wait at for graphics m_vkqueue submissions */
   //   VkPipelineStageFlags m_vkpipelinestageflagsSubmit = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
   //   // Contains command buffers and semaphores to be presented to the m_vkqueue
   //   VkSubmitInfo m_vksubmitinfo;
   //   std::vector<VkCommandBuffer> m_vkcommandbuffersDraw;
   //   // Command buffers used for rendering
   //   std::vector<VkCommandBuffer> m_vkcommandbuffersStencil;
   //   // Global render pass for frame buffer writes
   //   VkRenderPass m_vkrenderpass{ VK_NULL_HANDLE };
   //   // List of available frame buffers (same as number of swap chain images)
   //   std::vector<VkFramebuffer>m_vkframebuffers;
   //   // Active frame buffer index
   //   uint32_t m_uiCurrentBuffer = 0;
   //   // Descriptor set pool
   //   VkDescriptorPool m_vkdescriptorpool{ VK_NULL_HANDLE };
   //   // List of shader modules created (stored for cleanup)
   //   std::vector<VkShaderModule> m_vkshadermodules;
   //   // Pipeline cache object
   //   VkPipelineCache m_vkpipelinecache{ VK_NULL_HANDLE };
   //   // Wraps the swap chain to present images (framebuffers) to the windowing system
   //   //VulkanSwapChain swapChain;
   //   //// Synchronization semaphores
   //   //struct {
   //   //   // Swap chain image presentation
   //   //   VkSemaphore m_vksemaphorePresentComplete;
   //   //   // Command buffer submission and execution
   //   //   VkSemaphore m_vksemaphoreRenderComplete;
   //   //} semaphores;
   //   std::vector<VkFence> m_vkfencesWait;
   //   bool m_bRequiresStencil{ false };
   //public:
   //   bool m_bPrepared = false;
   //   bool m_bResized = false;
   //   bool m_bViewUpdated = false;
   //   uint32_t m_iWidth = 1280;
   //   uint32_t m_iHeight = 720;
   //   // frame_buffer for offscreen rendering
   //   struct FrameBufferAttachment {
   //      VkImage image;
   //      VkDeviceMemory mem;
   //      VkImageView view;
   //   };
   //   FrameBufferAttachment m_attachmentColor;
   //   vulkan::ui_overlay m_uioverlay;
   //   //CommandLineParser commandLineParser;

   //   /** @brief Last frame time measured using a high performance m_fTimer (if available) */
   //   float m_fFrameTimer = 1.0f;

   //   vks::Benchmark m_benchmark;

   //   /** @brief Encapsulated physical and logical vulkan device */
   //   ::vulkan::device* m_pvulkandevice = nullptr;

   //   ///** @brief Example m_settings that can be changed e.g. by command line arguments */
   //   //struct Settings {
   //   //   /** @brief Activates validation layers (and message output) when set to true */
   //   //   bool validation = false;
   //   //   /** @brief Set to true if m_bFullScreen mode has been requested via command line */
   //   //   bool m_bFullScreen = false;
   //   //   /** @brief Set to true if v-sync will be forced for the swapchain */
   //   //   bool vsync = false;
   //   //   /** @brief Enable UI overlay */
   //   //   bool overlay = true;
   //   //} m_settings;

   //   /** @brief State of gamepad input (only used on Android) */
   //   struct {
   //      glm::vec2 axisLeft = glm::vec2(0.0f);
   //      glm::vec2 axisRight = glm::vec2(0.0f);
   //   } m_gamepadstate;
   //   m_mousestate* m_pmousestate;
   //   ///** @brief State of mouse/touch input */
   //   //struct {
   //   //   struct {
   //   //      bool left = false;
   //   //      bool right = false;
   //   //      bool middle = false;
   //   //   } m_buttons;
   //   //   glm::vec2 position;
   //   //} m_mousestate;
   //   uint32_t m_uQueueFamilyIndex;
   //   VkFormat m_vkformatColorFB;
   //   VkClearColorValue m_vkclearcolorvalueDefault = { { 0.025f, 0.025f, 0.025f, 1.0f } };

   //   static std::vector<const char*> args;

   //   // Defines a frame rate independent m_fTimer value clamped from -1.0...1.0
   //   // For use in animations, rotations, etc.
   //   float m_fTimer = 0.0f;
   //   // Multiplier for speeding up (or slowing down) the global m_fTimer
   //   float m_fTimerSpeed = 0.25f;
   //   bool m_bPaused = false;

   //   Camera m_camera;

   //   //::string m_strTitle = "Vulkan Example"_ansi;
   //   //::string m_strWindowClassName = "vulkanExample"_ansi;
   //   ///uint32_t apiVersion = VK_API_VERSION_1_0;

   //   /** @brief Default depth stencil attachment used by the default render pass */
   //   struct {
   //      VkImage image;
   //      VkDeviceMemory memory;
   //      VkImageView view;
   //   } m_depthstencil{};



      //uint32_t m_iDestWidth;
      //uint32_t m_iDestHeight;
      //bool m_bResizing = false;
      //bool m_bQuit = false;
      //bool m_bConfigured = false;


      base_application_no_swap_chain();
      ~base_application_no_swap_chain() override;
      ///** @brief Setup the vulkan instance, enable required extensions and connect to the physical device (GPU) */
      //bool initVulkan();


      //std::string getWindowTitle();

      //std::string getShadersPath() const;


      //virtual void handleMouseMove(int32_t x, int32_t y) override;
      //virtual void nextFrame(const ::function < void(void*, int, int, int)>& callback);
      //virtual void updateOverlay();
      //virtual void createPipelineCache();
      //virtual void createCommandPool();
      //virtual void createSynchronizationPrimitives();
      //virtual void initSwapchain();
      //virtual void setupSwapChain();
      //virtual void create_command_buffers() override;
      //virtual void destroyCommandBuffers();
      ///** @brief (Virtual) Creates the application wide Vulkan instance */
      //virtual VkResult createInstance();
      ///** @brief (Pure virtual) Render function to be implemented by the sample application */
      //virtual void render(const ::function < void(void*, int, int, int)>& callback);
      ///** @brief (Virtual) Called after a key was pressed, can be used to do custom key handling */
      //virtual void keyPressed(uint32_t);
      ///** @brief (Virtual) Called after the mouse cursor moved and before internal happenings (like m_camera rotation) is handled */
      //virtual void mouseMoved(double x, double y, bool& handled);
      ///** @brief (Virtual) Called when the window has been m_bResized, can be used by the sample application to recreate resources */
      //virtual void windowResized();
      ///** @brief (Virtual) Called when resources have been recreated that require a rebuild of the command buffers (e.g. frame buffer), to be implemented by the sample application */
      //virtual void buildCommandBuffers();
      ///** @brief (Virtual) Setup default depth and stencil views */
      //virtual void setupDepthStencil();
      ///** @brief (Virtual) Setup default framebuffers for all requested swapchain images */
      //virtual void setupFrameBuffer();
      ///** @brief (Virtual) Setup a default renderpass */
      //virtual void setupRenderPass();
      ///** @brief (Virtual) Called after the physical device features have been read, can be used to set features to enable on the device */
      //virtual void getEnabledFeatures();
      ///** @brief (Virtual) Called after the physical device extensions have been read, can be used to enable extensions based on the supported extension listing*/
      //virtual void getEnabledExtensions();

      ///** @brief Prepares all Vulkan resources and functions required to run the sample */
      //virtual void prepare();

      ///** @brief Loads a SPIR-V shader file for the given shader stage */
      //VkPipelineShaderStageCreateInfo loadShader(std::string fileName, VkShaderStageFlagBits stage);

      //void windowResize();

      ///** @brief Entry point for the main render loop */
      //void renderLoop();

      ///** @brief Adds the drawing commands for the ImGui overlay to the given command buffer */
      //void drawUI(const VkCommandBuffer commandBuffer);

      ///** Prepare the next frame for workload submission by acquiring the next swap chain image */
      //void prepareFrame();
      ///** @brief Presents the current image to the swap chain */
      //void submitFrame();
      ///** @brief (Virtual) Default image acquire + submission and command buffer submission function */
      //virtual void renderFrame();

      ///** @brief (Virtual) Called when the UI overlay is updating, can be used to add custom elements to the overlay */
      //virtual void OnUpdateUIOverlay(vulkan::ui_overlay* overlay);

      //
      //    // OS specific
      // #if defined(_WIN32)
      //    HWND window;
      //    HINSTANCE windowInstance;
      // #elif defined(VK_USE_PLATFORM_ANDROID_KHR)
      //    // true if application has focused, false if moved to background
      //    bool focused = false;
      //    struct TouchPos {
      //       int32_t x;
      //       int32_t y;
      //    } touchPos;
      //    bool touchDown = false;
      //    double touchTimer = 0.0;
      //    long long lastTapTime = 0;
      // #elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
      //    void * view;
      // #if defined(VK_USE_PLATFORM_METAL_EXT)
      //    CAMetalLayer * metalLayer;
      // #endif
      // #if defined(VK_EXAMPLE_XCODE_GENERATED)
      //    bool m_bQuit = false;
      // #endif
      // #elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
      //    bool m_bQuit = false;
      //    IDirectFB * dfb = nullptr;
      //    IDirectFBDisplayLayer * layer = nullptr;
      //    IDirectFBWindow * window = nullptr;
      //    IDirectFBSurface * surface = nullptr;
      //    IDirectFBEventBuffer * event_buffer = nullptr;
      // #elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
      //    wl_display * display = nullptr;
      //    wl_registry * registry = nullptr;
      //    wl_compositor * compositor = nullptr;
      //    struct xdg_wm_base * shell = nullptr;
      //    wl_seat * seat = nullptr;
      //    wl_pointer * pointer = nullptr;
      //    wl_keyboard * keyboard = nullptr;
      //    wl_surface * surface = nullptr;
      //    struct xdg_surface * xdg_surface;
      //    struct xdg_toplevel * xdg_toplevel;
      //    bool m_bQuit = false;
      //    bool m_bConfigured = false;
      //
      // #elif defined(_DIRECT2DISPLAY)
      //    bool m_bQuit = false;
      // #elif defined(VK_USE_PLATFORM_XCB_KHR)
      //    bool m_bQuit = false;
      //    xcb_connection_t * connection;
      //    xcb_screen_t * screen;
      //    xcb_window_t window;
      //    xcb_intern_atom_reply_t * atom_wm_delete_window;
      // #elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
      //    bool m_bQuit = false;
      // #elif defined(VK_USE_PLATFORM_SCREEN_QNX)
      //    screen_context_t screen_context = nullptr;
      //    screen_window_t screen_window = nullptr;
      //    screen_event_t screen_event = nullptr;
      //    bool m_bQuit = false;
      // #endif
      //
      //    /** @brief Default base class constructor */
      //
      // #if defined(_WIN32)
      //    void setupConsole(std::string title);
      //    void setupDPIAwareness();
      //    HWND setupWindow(HINSTANCE hinstance, WNDPROC wndproc);
      //    //void handleMessages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
      // #elif defined(VK_USE_PLATFORM_ANDROID_KHR)
      //    static int32_t handleAppInput(struct android_app * app, AInputEvent * happening);
      //    static void handleAppCommand(android_app * app, int32_t cmd);
      // #elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
      //    void * setupWindow(void * view);
      //    void displayLinkOutputCb();
      //    void mouseDragged(float x, float y);
      //    void windowWillResize(float x, float y);
      //    void windowDidResize();
      // #elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
      //    IDirectFBSurface * setupWindow();
      //    void handleEvent(const DFBWindowEvent * happening);
      // #elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
      //    struct xdg_surface * setupWindow();
      //    void initWaylandConnection();
      //    void setSize(int m_iWidth, int m_iHeight);
      //    static void registryGlobalCb(void * data, struct wl_registry * registry,
      //          uint32_t name, const char * interface, uint32_t version);
      //    void registryGlobal(struct wl_registry * registry, uint32_t name,
      //          const char * interface, uint32_t version);
      //    static void registryGlobalRemoveCb(void * data, struct wl_registry * registry,
      //          uint32_t name);
      //    static void seatCapabilitiesCb(void * data, wl_seat * seat, uint32_t caps);
      //    void seatCapabilities(wl_seat * seat, uint32_t caps);
      //    static void pointerEnterCb(void * data, struct wl_pointer * pointer,
      //          uint32_t serial, struct wl_surface * surface, wl_fixed_t sx,
      //          wl_fixed_t sy);
      //    static void pointerLeaveCb(void * data, struct wl_pointer * pointer,
      //          uint32_t serial, struct wl_surface * surface);
      //    static void pointerMotionCb(void * data, struct wl_pointer * pointer,
      //          uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
      //    void pointerMotion(struct wl_pointer * pointer,
      //          uint32_t time, wl_fixed_t sx, wl_fixed_t sy);
      //    static void pointerButtonCb(void * data, struct wl_pointer * wl_pointer,
      //          uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
      //    void pointerButton(struct wl_pointer * wl_pointer,
      //          uint32_t serial, uint32_t time, uint32_t button, uint32_t state);
      //    static void pointerAxisCb(void * data, struct wl_pointer * wl_pointer,
      //          uint32_t time, uint32_t axis, wl_fixed_t value);
      //    void pointerAxis(struct wl_pointer * wl_pointer,
      //          uint32_t time, uint32_t axis, wl_fixed_t value);
      //    static void keyboardKeymapCb(void * data, struct wl_keyboard * keyboard,
      //          uint32_t format, int fd, uint32_t size);
      //    static void keyboardEnterCb(void * data, struct wl_keyboard * keyboard,
      //          uint32_t serial, struct wl_surface * surface, struct wl_array * keys);
      //    static void keyboardLeaveCb(void * data, struct wl_keyboard * keyboard,
      //          uint32_t serial, struct wl_surface * surface);
      //    static void keyboardKeyCb(void * data, struct wl_keyboard * keyboard,
      //          uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
      //    void keyboardKey(struct wl_keyboard * keyboard,
      //          uint32_t serial, uint32_t time, uint32_t key, uint32_t state);
      //    static void keyboardModifiersCb(void * data, struct wl_keyboard * keyboard,
      //          uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched,
      //          uint32_t mods_locked, uint32_t group);
      //
      // #elif defined(_DIRECT2DISPLAY)
      //    //
      // #elif defined(VK_USE_PLATFORM_XCB_KHR)
      //    xcb_window_t setupWindow();
      //    void initxcbConnection();
      //    void handleEvent(const xcb_generic_event_t * happening);
      // #elif defined(VK_USE_PLATFORM_SCREEN_QNX)
      //    void setupWindow();
      //    void handleEvent();
      // #else
      //    void setupWindow();
      // #endif

      // #if defined(_WIN32)
      //    virtual void OnHandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
      // #endif
   };


} // namespace vulkan1



//#include "Entrypoints.h"