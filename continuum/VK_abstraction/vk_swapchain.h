#pragma once


#include "vk_renderpass.h"


namespace vkc
{


   class VkcSwapChain :
      virtual public VkcRenderPass
   {
   public:
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      VkcSwapChain(VkcDevice* pvkcdeviceRef, VkExtent2D windowExtent);
      VkcSwapChain(VkcDevice* pvkcdeviceRef, VkExtent2D windowExtent, ::pointer <VkcRenderPass>previous);
      ~VkcSwapChain();

      VkcSwapChain(const VkcSwapChain&) = delete;
      VkcSwapChain& operator=(const VkcSwapChain&) = delete;

      //VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
      //VkRenderPass getRenderPass() { return m_vkrenderpass; }
      //VkImageView getImageView(int index) { return swapChainImageViews[index]; }
      //size_t imageCount() { return swapChainImages.size(); }
      //VkFormat getSwapChainImageFormat() { return swapChainImageFormat; }
      //VkExtent2D getSwapChainExtent() { return m_extent; }
      //uint32_t width() { return m_extent.width; }
      //uint32_t height() { return m_extent.height; }

      //float extentAspectRatio() {
      //   return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
      //}
      VkFormat findDepthFormat();

      VkResult acquireNextImage(uint32_t* imageIndex);
      VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

      //bool compareSwapFormats(const VkcSwapChain& m_swapchain) const {
      //   return m_swapchain.swapChainDepthFormat == swapChainDepthFormat &&
      //      m_swapchain.swapChainImageFormat == swapChainImageFormat;
      //}

   public:
      void init();
      void createRenderPassImpl();
      void createImageViews();
      void createDepthResources();
      void createRenderPass();
      void createFramebuffers();
      void createSyncObjects();

      // Helper functions
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(
         const std::vector<VkSurfaceFormatKHR>& availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(
         const std::vector<VkPresentModeKHR>& availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

      /*VkFormat swapChainImageFormat;
      VkFormat swapChainDepthFormat;
      VkExtent2D m_extent;

      std::vector<VkFramebuffer> swapChainFramebuffers;
      VkRenderPass m_vkrenderpass;

      std::vector<VkImage> depthImages;
      std::vector<VkDeviceMemory> depthImageMemorys;
      std::vector<VkImageView> depthImageViews;
      std::vector<VkImage> swapChainImages;
      std::vector<VkImageView> swapChainImageViews;

      ::pointer < VkcDevice > m_pvkcdevice;
      VkExtent2D windowExtent;*/

      VkSwapchainKHR m_vkswapchain;
      //::pointer<VkcSwapChain> oldSwapChain;

      //std::vector<VkSemaphore> imageAvailableSemaphores;
      //std::vector<VkSemaphore> renderFinishedSemaphores;
      //std::vector<VkFence> inFlightFences;
      //std::vector<VkFence> imagesInFlight;
      //size_t currentFrame = 0;

   };


}  // namespace vkc


