/*
* UI overlay class using ImGui
*
* Copyright (C) 2017-2024 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <sstream>
#include <iomanip>

#include "_vulkan.h"
#include "tools.h"
#include "debug.h"
#include "buffer.h"
#include "device.h"

#include "imgui/imgui.h"

#if defined(__ANDROID__)
#include "VulkanAndroid.h"
#endif


namespace vulkan1
{


	class ui_overlay
	{
	public:


		vulkan::device* device{ nullptr };
		VkQueue m_vkqueue{ VK_NULL_HANDLE };

		VkSampleCountFlagBits rasterizationSamples{ VK_SAMPLE_COUNT_1_BIT };
		uint32_t subpass{ 0 };

		vulkan::buffer vertexBuffer;
		vulkan::buffer indexBuffer;
		int32_t vertexCount{ 0 };
		int32_t indexCount{ 0 };

		std::vector<VkPipelineShaderStageCreateInfo> shaders;

		VkDescriptorPool m_vkdescriptorpool{ VK_NULL_HANDLE };
		VkDescriptorSetLayout descriptorSetLayout{ VK_NULL_HANDLE };
		VkDescriptorSet descriptorSet{ VK_NULL_HANDLE };
		VkPipelineLayout pipelineLayout{ VK_NULL_HANDLE };
		VkPipeline pipeline{ VK_NULL_HANDLE };

		VkDeviceMemory fontMemory{ VK_NULL_HANDLE };
		VkImage fontImage{ VK_NULL_HANDLE };
		VkImageView fontView{ VK_NULL_HANDLE };
		VkSampler sampler{ VK_NULL_HANDLE };

		struct PushConstBlock {
			glm::vec2 scale;
			glm::vec2 translate;
		} pushConstBlock;

		bool visible{ true };
		bool updated{ false };
		float scale{ 1.0f };
		float updateTimer{ 0.0f };

		ui_overlay();
		~ui_overlay();

		void preparePipeline(const VkPipelineCache m_vkpipelinecache, const VkRenderPass m_vkrenderpass, const VkFormat colorFormat, const VkFormat m_vkformatDepth);
		void prepareResources();

		bool update();
		void draw(const VkCommandBuffer commandBuffer);
		void resize(uint32_t m_iWidth, uint32_t m_iHeight);

		void freeResources();

		bool header(const char* caption);
		bool checkBox(const char* caption, bool* value);
		bool checkBox(const char* caption, int32_t* value);
		bool radioButton(const char* caption, bool value);
		bool inputFloat(const char* caption, float* value, float step, const char * format = "%.3f"_ansi);
		bool sliderFloat(const char* caption, float* value, float min, float max);
		bool sliderInt(const char* caption, int32_t* value, int32_t min, int32_t max);
		bool comboBox(const char* caption, int32_t* itemindex, std::vector<std::string> items);
		bool button(const char* caption);
		bool colorPicker(const char* caption, float* color);
		void text(const char* formatstr, ...);
	};


} // namespace vulkan1




