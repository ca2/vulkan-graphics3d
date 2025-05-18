#include "framework.h"
#include "basic_render_system.h"
// libs
#define GLM_FORCE_RADIANS	
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


#include <string>
#include <array>
#include <cassert>
#include <stdexcept>


namespace graphics3d_vulkan {
	struct SimplePushConstantData {
		glm::mat4 modelMatrix{ 1.f };
		glm::mat4 normalMatrix{ 1.f };
	};


	SimpleRenderSystem::SimpleRenderSystem(context * pdevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout)
		: m_pcontext{ pdevice } {
		createPipelineLayout(globalSetLayout);
		createPipeline(renderPass);

	}

	SimpleRenderSystem::~SimpleRenderSystem() {
		vkDestroyPipelineLayout(m_pcontext->logicalDevice(), pipelineLayout, nullptr);
	}



	void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalSetLayout) {

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		std::vector<VkDescriptorSetLayout> descriptorSetLayouts{ globalSetLayout };

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
		pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
		if (vkCreatePipelineLayout(m_pcontext->logicalDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
			VK_SUCCESS) {
			throw std::runtime_error("Failed to create pipeline layout");
		}

	}


	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) 
	{

		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

		PipelineConfigInfo pipelineConfig{};
		pipeline::defaultPipelineConfigInfo(pipelineConfig);

		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;

		// Construct paths using PROJECT_ROOT_DIR
		std::string vertShaderPath = "matter://Shaders/SpirV/vert.vert.spv";
		std::string fragShaderPath = "matter://Shaders/SpirV/frag.frag.spv";

		m_ppipeline = __allocate pipeline();
		
		m_ppipeline->initialize_pipeline(
			m_pcontext,
			vertShaderPath.c_str(),
			fragShaderPath.c_str(),
			pipelineConfig
		);
	}


	void SimpleRenderSystem::renderGameObjects(FrameInfo& frameInfo) {

		m_ppipeline->bind(frameInfo.commandBuffer);
		vkCmdBindDescriptorSets(
			frameInfo.commandBuffer,
			VK_PIPELINE_BIND_POINT_GRAPHICS,
			pipelineLayout,
			0, 1,
			&frameInfo.globalDescriptorSet,
			0, nullptr
		);

		for (auto& kv : frameInfo.gameObjects) {
			auto& obj = kv.element2();
			SimplePushConstantData push{};
			push.modelMatrix = obj->m_transform.mat4();
			push.normalMatrix = obj->m_transform.normalMatrix();

			vkCmdPushConstants(
				frameInfo.commandBuffer,
				pipelineLayout,
				VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
				0,
				sizeof(SimplePushConstantData),
				&push);

			if (obj->m_pmodel)
			{
				obj->m_pmodel->bind(&frameInfo);
				obj->m_pmodel->draw(&frameInfo);
			}
		}
	}
}