#pragma once
 
//#include "AppCore/vk_window.h"
#include "vulkan-land/landen/vulkan/vk_container.h"
#include "VK_abstraction/vk_device.h"
//#include "VK_abstraction/vk_offscreen.h"
//#include "VK_abstraction/vk_swapchain.h"
#include "VK_abstraction/vk_renderpass.h"



#include <memory>
#include <vector>
#include <cassert>
 
constexpr unsigned int FRAME_OVERLAP = 2;


namespace vkc {
	class Renderer:
	virtual public ::particle{
	public:
		bool m_bOffScreen = true;
		//Renderer(VkWindow &window, VkcDevice * pvkcdevice);
		Renderer();
		~Renderer();


		void initialize_renderer(vkc::VkContainer * pvkccontainer,  VkcDevice* pvkcdevice);

		Renderer(const Renderer&) = delete;
		Renderer& operator=(const Renderer&) = delete;


		VkRenderPass getRenderPass() const 
		{
		
			return m_pvkcrenderpass->getRenderPass();
			//return m_bOffScreen ?
				//m_pvkcoffscreen->getRenderPass():
				//m_pvkcswapchain->getRenderPass(); 

		}

		void sample();

		float getAspectRatio() const
		{

			//if (m_bOffScreen)
			{

				return m_pvkcrenderpass->extentAspectRatio();

			}
			//else
			//{

			//	return m_pvkcswapchain->extentAspectRatio();

			//}

		}


		bool isFrameInProgress() const { return isFrameStarted; }


		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame not in progress");
			return commandBuffers[currentFrameIndex];
		}

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currentFrameIndex;
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginRenderPass(VkCommandBuffer commandBuffer);
		void endRenderPass(VkCommandBuffer commandBuffer);
	public:
		void create_command_buffers() override;

		void free_command_buffers() override;
		//void recreateSwapchain();
		void defer_layout();

		//void prepareOffScreen();

		struct OffScreenSampler :
			virtual public ::particle
		{

			VkExtent2D			m_vkextent2d;
			VkDeviceMemory		m_vkdevicememory;
			VkImage				m_vkimage;

			::pointer < VkcDevice > m_pvkcdevice;





			OffScreenSampler();
			~OffScreenSampler();


			void initialize_offscreen_sampler(VkcDevice* pvkcdevice);

			void clear();
			void update(VkExtent2D vkextent2d);
			void destroy();

			void sample(VkImage vkimage);

			void send_sample();

		};

		::pointer < vkc::VkContainer >	m_pvkcontainer;
		::pointer < VkcDevice >				m_pvkcdevice;
		::pointer < OffScreenSampler >	m_poffscreensampler;
		//::pointer<VkcSwapChain>			m_pvkcswapchain;
		//::pointer<VkcOffScreen>			m_pvkcoffscreen;
		::pointer<VkcRenderPass>			m_pvkcrenderpass;
		std::vector<VkCommandBuffer>	commandBuffers;
		VkExtent2D m_extentRenderer;
		uint32_t currentImageIndex;
		int currentFrameIndex = 0;
		bool isFrameStarted = false;
	
	};
}

