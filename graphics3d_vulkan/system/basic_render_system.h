#pragma once

#include "context.h"
#include "pipeline.h"
#include "scene_object.h"
#include "frame_info.h"
#include "app-cube/cube/graphics3d/camera.h"



#include <memory>
#include <vector>



namespace graphics3d_vulkan 
{

	class SimpleRenderSystem:
	virtual public ::particle 
	{
	public:
		SimpleRenderSystem(context *pvkcdevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(FrameInfo& frameInfo);
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

		context* m_pcontext;


		::pointer<pipeline> m_ppipeline;
		VkPipelineLayout pipelineLayout;
	};
}
