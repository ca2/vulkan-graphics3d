// From vk_swapchain by camilo on 2025-05-09 01:40 <3ThomasBorregaardSorensen!!
#pragma once

#include "vk_device.h"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <string>
#include <vector>
#include <memory>


namespace vkc {

   class VkcRenderPass :
      virtual public ::particle
   {
   public:
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      VkcRenderPass(VkcDevice* pvkcdeviceRef, VkExtent2D windowExtent);
      VkcRenderPass(VkcDevice* pvkcdeviceRef, VkExtent2D windowExtent, ::pointer <VkcRenderPass>previous);
      ~VkcRenderPass();

      VkcRenderPass(const VkcRenderPass&) = delete;
      VkcRenderPass& operator=(const VkcRenderPass&) = delete;

      VkFramebuffer getFrameBuffer(int index) { return m_framebuffers[index]; }
      VkRenderPass getRenderPass() { return m_vkrenderpass; }
      VkImageView getImageView(int index) { return m_imageviews[index]; }
      size_t imageCount() { return m_images.size(); }
      VkFormat getImageFormat() { return m_formatImage; }
      VkExtent2D getExtent() { return m_extent; }
      uint32_t width() { return m_extent.width; }
      uint32_t height() { return m_extent.height; }

      float extentAspectRatio() {
         return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
      }
      virtual VkFormat findDepthFormat();

      virtual VkResult acquireNextImage(uint32_t* imageIndex);
      virtual VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

      bool compareFormats(const VkcRenderPass& renderpass) const {
         return renderpass.m_formatDepth == m_formatDepth &&
            renderpass.m_formatImage == m_formatImage;
      }

      virtual void init();
      virtual void createRenderPassImpl();
      virtual void createImageViews();
      virtual void createDepthResources();
      virtual void createRenderPass();
      virtual void createFramebuffers();
      virtual void createSyncObjects();
      //virtual void defer_layout();
      //// Helper functions
      //VkSurfaceFormatKHR chooseSwapSurfaceFormat(
      //   const std::vector<VkSurfaceFormatKHR>& availableFormats);
      //VkPresentModeKHR chooseSwapPresentMode(
      //   const std::vector<VkPresentModeKHR>& availablePresentModes);
      //VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

      VkFormat m_formatImage;
      VkFormat m_formatDepth;
      VkExtent2D m_extent;

      std::vector<VkFramebuffer> m_framebuffers;
      VkRenderPass m_vkrenderpass;

      std::vector<VkImage> depthImages;
      std::vector<VkDeviceMemory> depthImageMemorys;
      std::vector<VkImageView> depthImageViews;
      std::vector<VkImage> m_images;
      std::vector<VkImageView> m_imageviews;

      VkcDevice* m_pvkcdevice;
      VkExtent2D windowExtent;

      //VkSwapchainKHR swapChain;
      ::pointer<VkcRenderPass> m_pvkcrenderpassOld;

      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      size_t currentFrame = 0;
   };

}  // namespace vkc
