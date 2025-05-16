// From vk_swapchain by camilo on 2025-05-09 <3ThomasBorregaardSorensen!!
#pragma once

#include "vk_renderpass.h"

//// vulkan headers
//#include <vulkan/vulkan.h>
//
//// std lib headers
//#include <string>
//#include <vector>
//#include <memory>


namespace graphics3d_vulkan 
{

	class VkcOffScreen :
		virtual public VkcRenderPass
	{
	public:


		static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

		VkcOffScreen(VkcDevice* pvkcdeviceRef, VkExtent2D windowExtent);
		VkcOffScreen(VkcDevice* pvkcdeviceRef, VkExtent2D windowExtent, ::pointer <VkcRenderPass>previous);
		~VkcOffScreen();

		VkcOffScreen(const VkcOffScreen&) = delete;
		VkcOffScreen& operator=(const VkcOffScreen&) = delete;

		//VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
		//VkRenderPass getRenderPass() { return m_vkrenderpass; }
		//VkImageView getImageView(int index) { return m_imageview[index]; }
		//size_t imageCount() { return m_images.size(); }
		//VkFormat getImageFormat() { return imageFormat; }
		//VkExtent2D getExtent() { return m_extent; }
		//uint32_t width() { return m_extent.width; }
		//uint32_t height() { return m_extent.height; }

		//float extentAspectRatio() {
		//	return static_cast<float>(m_extent.width) / static_cast<float>(m_extent.height);
		//}
		VkFormat findDepthFormat();

		VkResult acquireNextImage(uint32_t* imageIndex) override;
		VkResult submitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imageIndex);

		//bool compareSwapFormats(const VkcOffScreen& swapChain) const {
		//	return swapChain.depthFormat == depthFormat &&
		//		swapChain.imageFormat == imageFormat;
		//}

	public:
		void init();
		void createRenderPassImpl();
		void createImageViews();
		void createDepthResources();
		void createRenderPass();
		void createFramebuffers();
		void createSyncObjects();

		//// Helper functions
		//VkSurfaceFormatKHR chooseSwapSurfaceFormat(
		//	const std::vector<VkSurfaceFormatKHR>& availableFormats);
		//VkPresentModeKHR chooseSwapPresentMode(
		//	const std::vector<VkPresentModeKHR>& availablePresentModes);
		//VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		//VkFormat imageFormat;
		//VkFormat depthFormat;
		//VkExtent2D m_extent;

		//std::vector<VkFramebuffer> swapChainFramebuffers;
		//VkRenderPass m_vkrenderpass;

		//std::vector<VkImage> depthImages;
		//std::vector<VkDeviceMemory> depthImageMemorys;
		//std::vector<VkImageView> depthImageViews;
		VkSampler m_vksampler;
		 std::vector<VkDeviceMemory> m_imagememories;
		//std::vector<VkImage> swapChainImages;
		//std::vector<VkImageView> swapChainImageViews;

		//VkcDevice* m_pvkcdevice;
		//VkExtent2D windowExtent;

		////VkSwapchainKHR swapChain;
		//::pointer<VkcOffScreen> oldOffScreen;

		//std::vector<VkSemaphore> imageAvailableSemaphores;
		//std::vector<VkSemaphore> renderFinishedSemaphores;
		//std::vector<VkFence> inFlightFences;
		//std::vector<VkFence> imagesInFlight;
		//size_t currentFrame = 0;
	};

}  // namespace graphics3d_vulkan


