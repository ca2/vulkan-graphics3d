#pragma once
 
//#include "AppCore/vk_window.h"
//#include "app-cube/cube/container.h"
#include "context.h"
//#include "offscreen.h"
//#include "swapchain.h"
#include "render_pass.h"



#include <memory>
#include <vector>
#include <cassert>
 
constexpr unsigned int FRAME_OVERLAP = 2;


namespace graphics3d_vulkan
{


	class CLASS_DECL_GRAPHICS3D_VULKAN renderer:
	virtual public ::particle
	{
	public:
		bool m_bOffScreen = true;
		//renderer(VkWindow &window, context * pvkcdevice);
		renderer();
		~renderer();


		virtual void initialize_renderer(::cube::impact * pimpact,  ::graphics3d::context* pcontext);



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
		void createCommandBuffers();

		void freeCommandBuffers();
		//void recreateSwapchain();
		void defer_layout();

		//void prepareOffScreen();

		struct OffScreenSampler :
			virtual public ::particle
		{

			VkExtent2D			m_vkextent2d;
			VkDeviceMemory		m_vkdevicememory;
			VkImage				m_vkimage;

			::pointer < context > m_pcontext;





			OffScreenSampler();
			~OffScreenSampler();


			void initialize_offscreen_sampler(::graphics3d::context* pcontext);

			void clear();
			void update(VkExtent2D vkextent2d);
			void destroy();

			void sample(VkImage vkimage);

			void send_sample();

		};

		::pointer < ::cube::impact >	m_pimpact;
		::pointer < context >				m_pcontext;
		::pointer < OffScreenSampler >	m_poffscreensampler;
		//::pointer<swap_chain_render_pass>			m_pvkcswapchain;
		//::pointer<offscreen_render_pass>			m_pvkcoffscreen;
		::pointer<render_pass>			m_pvkcrenderpass;
		std::vector<VkCommandBuffer>	commandBuffers;
		VkExtent2D m_extentRenderer;
		uint32_t currentImageIndex;
		int currentFrameIndex = 0;
		bool isFrameStarted = false;
	
	};
}

