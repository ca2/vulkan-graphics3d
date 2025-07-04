/*
* Extended sample base class for ray tracing based samples
*
* Copyright (C) 2020-2024 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "framework.h"
#include "raytracing_application.h"
#include <array>

namespace vulkan1
{

	void raytracing_application::setupRenderPass()
	{
		// Update the default render pass with different color attachment load ops to keep attachment contents
		// With this change, we can e.g. draw an UI on top of the ray traced scene

		vkDestroyRenderPass(m_vkdevice, m_vkrenderpass, nullptr);

		VkAttachmentLoadOp colorLoadOp{ VK_ATTACHMENT_LOAD_OP_LOAD };
		VkImageLayout colorInitialLayout{ VK_IMAGE_LAYOUT_PRESENT_SRC_KHR };

		if (rayQueryOnly) {
			// For samples that use ray queries with rasterization, we need to use a setup similar to the non-ray tracing samples
			colorLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorInitialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		}

		std::array<VkAttachmentDescription, 2> attachments = {};
		// Color attachment
		attachments[0].format = m_swapchain.colorFormat;
		attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[0].loadOp = colorLoadOp;
		attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[0].initialLayout = colorInitialLayout;
		attachments[0].finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		// Depth attachment
		attachments[1].format = m_vkformatDepth;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorReference = {};
		colorReference.attachment = 0;
		colorReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthReference = {};
		depthReference.attachment = 1;
		depthReference.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpassDescription = {};
		subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpassDescription.colorAttachmentCount = 1;
		subpassDescription.pColorAttachments = &colorReference;
		subpassDescription.pDepthStencilAttachment = &depthReference;
		subpassDescription.inputAttachmentCount = 0;
		subpassDescription.pInputAttachments = nullptr;
		subpassDescription.preserveAttachmentCount = 0;
		subpassDescription.pPreserveAttachments = nullptr;
		subpassDescription.pResolveAttachments = nullptr;

		// Subpass dependencies for layout transitions
		std::array<VkSubpassDependency, 2> dependencies{};

		dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[0].dstSubpass = 0;
		dependencies[0].srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[0].srcAccessMask = VK_ACCESS_NONE_KHR;
		dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		dependencies[1].srcSubpass = 0;
		dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
		dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
		dependencies[1].dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
		dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
		dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
		dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpassDescription;
		renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
		renderPassInfo.pDependencies = dependencies.data();
		VK_CHECK_RESULT(vkCreateRenderPass(m_vkdevice, &renderPassInfo, nullptr, &m_vkrenderpass));

	}


	void raytracing_application::setupFrameBuffer()
	{
		VkImageView attachments[2];

		// Depth/Stencil attachment is the same for all frame buffers
		attachments[1] = m_depthstencil.view;

		VkFramebufferCreateInfo frameBufferCreateInfo = {};
		frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		frameBufferCreateInfo.pNext = NULL;
		frameBufferCreateInfo.renderPass = m_vkrenderpass;
		frameBufferCreateInfo.attachmentCount = 2;
		frameBufferCreateInfo.pAttachments = attachments;
		frameBufferCreateInfo.width = m_iWidth;
		frameBufferCreateInfo.height = m_iHeight;
		frameBufferCreateInfo.layers = 1;

		// Create frame buffers for every swap chain image
		m_vkframebuffers.resize(m_swapchain.imageCount);
		for (uint32_t i = 0; i < m_vkframebuffers.size(); i++)
		{
			attachments[0] = m_swapchain.buffers[i].view;
			VK_CHECK_RESULT(vkCreateFramebuffer(m_vkdevice, &frameBufferCreateInfo, nullptr, &m_vkframebuffers[i]));
		}
	}

	void raytracing_application::enableExtensions()
	{
		// Require Vulkan 1.1
		m_uApiVersion = VK_API_VERSION_1_1;

		// Ray tracing related extensions required by this sample
		m_pszaEnabledDeviceExtensions.add(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
		if (!rayQueryOnly) {
			m_pszaEnabledDeviceExtensions.add(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
		}

		// Required by VK_KHR_acceleration_structure
		m_pszaEnabledDeviceExtensions.add(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME);
		m_pszaEnabledDeviceExtensions.add(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
		m_pszaEnabledDeviceExtensions.add(VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME);

		// Required for VK_KHR_ray_tracing_pipeline
		m_pszaEnabledDeviceExtensions.add(VK_KHR_SPIRV_1_4_EXTENSION_NAME);

		// Required by VK_KHR_spirv_1_4
		m_pszaEnabledDeviceExtensions.add(VK_KHR_SHADER_FLOAT_CONTROLS_EXTENSION_NAME);
	}

	raytracing_application::ScratchBuffer raytracing_application::createScratchBuffer(VkDeviceSize size)
	{
		ScratchBuffer scratchBuffer{};
		// Buffer and memory
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
		VK_CHECK_RESULT(vkCreateBuffer(m_pvulkandevice->logicalDevice, &bufferCreateInfo, nullptr, &scratchBuffer.handle));
		VkMemoryRequirements memoryRequirements{};
		vkGetBufferMemoryRequirements(m_pvulkandevice->logicalDevice, scratchBuffer.handle, &memoryRequirements);
		VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo{};
		memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
		memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
		VkMemoryAllocateInfo memoryAllocateInfo = {};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = m_pvulkandevice->getMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(m_pvulkandevice->logicalDevice, &memoryAllocateInfo, nullptr, &scratchBuffer.memory));
		VK_CHECK_RESULT(vkBindBufferMemory(m_pvulkandevice->logicalDevice, scratchBuffer.handle, scratchBuffer.memory, 0));
		// Buffer m_vkdevice address
		VkBufferDeviceAddressInfoKHR bufferDeviceAddresInfo{};
		bufferDeviceAddresInfo.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		bufferDeviceAddresInfo.buffer = scratchBuffer.handle;
		scratchBuffer.deviceAddress = vkGetBufferDeviceAddressKHR(m_pvulkandevice->logicalDevice, &bufferDeviceAddresInfo);
		return scratchBuffer;
	}

	void raytracing_application::deleteScratchBuffer(ScratchBuffer& scratchBuffer)
	{
		if (scratchBuffer.memory != VK_NULL_HANDLE) {
			vkFreeMemory(m_pvulkandevice->logicalDevice, scratchBuffer.memory, nullptr);
		}
		if (scratchBuffer.handle != VK_NULL_HANDLE) {
			vkDestroyBuffer(m_pvulkandevice->logicalDevice, scratchBuffer.handle, nullptr);
		}
	}

	void raytracing_application::createAccelerationStructure(AccelerationStructure& accelerationStructure, VkAccelerationStructureTypeKHR type, VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo)
	{
		// Buffer and memory
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = buildSizeInfo.accelerationStructureSize;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT;
		VK_CHECK_RESULT(vkCreateBuffer(m_pvulkandevice->logicalDevice, &bufferCreateInfo, nullptr, &accelerationStructure.buffer));
		VkMemoryRequirements memoryRequirements{};
		vkGetBufferMemoryRequirements(m_pvulkandevice->logicalDevice, accelerationStructure.buffer, &memoryRequirements);
		VkMemoryAllocateFlagsInfo memoryAllocateFlagsInfo{};
		memoryAllocateFlagsInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_FLAGS_INFO;
		memoryAllocateFlagsInfo.flags = VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR;
		VkMemoryAllocateInfo memoryAllocateInfo{};
		memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memoryAllocateInfo.pNext = &memoryAllocateFlagsInfo;
		memoryAllocateInfo.allocationSize = memoryRequirements.size;
		memoryAllocateInfo.memoryTypeIndex = m_pvulkandevice->getMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(m_pvulkandevice->logicalDevice, &memoryAllocateInfo, nullptr, &accelerationStructure.memory));
		VK_CHECK_RESULT(vkBindBufferMemory(m_pvulkandevice->logicalDevice, accelerationStructure.buffer, accelerationStructure.memory, 0));
		// Acceleration structure
		VkAccelerationStructureCreateInfoKHR accelerationStructureCreate_info{};
		accelerationStructureCreate_info.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
		accelerationStructureCreate_info.buffer = accelerationStructure.buffer;
		accelerationStructureCreate_info.size = buildSizeInfo.accelerationStructureSize;
		accelerationStructureCreate_info.type = type;
		vkCreateAccelerationStructureKHR(m_pvulkandevice->logicalDevice, &accelerationStructureCreate_info, nullptr, &accelerationStructure.handle);
		// AS m_vkdevice address
		VkAccelerationStructureDeviceAddressInfoKHR accelerationDeviceAddressInfo{};
		accelerationDeviceAddressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
		accelerationDeviceAddressInfo.accelerationStructure = accelerationStructure.handle;
		accelerationStructure.deviceAddress = vkGetAccelerationStructureDeviceAddressKHR(m_pvulkandevice->logicalDevice, &accelerationDeviceAddressInfo);
	}

	void raytracing_application::deleteAccelerationStructure(AccelerationStructure& accelerationStructure)
	{
		vkFreeMemory(m_vkdevice, accelerationStructure.memory, nullptr);
		vkDestroyBuffer(m_vkdevice, accelerationStructure.buffer, nullptr);
		vkDestroyAccelerationStructureKHR(m_vkdevice, accelerationStructure.handle, nullptr);
	}

	uint64_t raytracing_application::getBufferDeviceAddress(VkBuffer buffer)
	{
		VkBufferDeviceAddressInfoKHR bufferDeviceAI{};
		bufferDeviceAI.sType = VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO;
		bufferDeviceAI.buffer = buffer;
		return vkGetBufferDeviceAddressKHR(m_pvulkandevice->logicalDevice, &bufferDeviceAI);
	}

	void raytracing_application::createStorageImage(VkFormat format, VkExtent3D extent)
	{
		// Release ressources if image is to be recreated
		if (storageImage.image != VK_NULL_HANDLE) {
			vkDestroyImageView(m_vkdevice, storageImage.view, nullptr);
			vkDestroyImage(m_vkdevice, storageImage.image, nullptr);
			vkFreeMemory(m_vkdevice, storageImage.memory, nullptr);
			storageImage = {};
		}

		VkImageCreateInfo image = vks::initializers::imageCreateInfo();
		image.imageType = VK_IMAGE_TYPE_2D;
		image.format = format;
		image.extent = extent;
		image.mipLevels = 1;
		image.arrayLayers = 1;
		image.samples = VK_SAMPLE_COUNT_1_BIT;
		image.tiling = VK_IMAGE_TILING_OPTIMAL;
		image.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_STORAGE_BIT;
		image.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		VK_CHECK_RESULT(vkCreateImage(m_pvulkandevice->logicalDevice, &image, nullptr, &storageImage.image));

		VkMemoryRequirements memReqs;
		vkGetImageMemoryRequirements(m_pvulkandevice->logicalDevice, storageImage.image, &memReqs);
		VkMemoryAllocateInfo memoryAllocateInfo = vks::initializers::memoryAllocateInfo();
		memoryAllocateInfo.allocationSize = memReqs.size;
		memoryAllocateInfo.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		VK_CHECK_RESULT(vkAllocateMemory(m_pvulkandevice->logicalDevice, &memoryAllocateInfo, nullptr, &storageImage.memory));
		VK_CHECK_RESULT(vkBindImageMemory(m_pvulkandevice->logicalDevice, storageImage.image, storageImage.memory, 0));

		VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
		colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
		colorImageView.format = format;
		colorImageView.subresourceRange = {};
		colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		colorImageView.subresourceRange.baseMipLevel = 0;
		colorImageView.subresourceRange.levelCount = 1;
		colorImageView.subresourceRange.baseArrayLayer = 0;
		colorImageView.subresourceRange.layerCount = 1;
		colorImageView.image = storageImage.image;
		VK_CHECK_RESULT(vkCreateImageView(m_pvulkandevice->logicalDevice, &colorImageView, nullptr, &storageImage.view));

		VkCommandBuffer cmdBuffer = m_pvulkandevice->createCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY, true);
		vks::tools::setImageLayout(cmdBuffer, storageImage.image,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_GENERAL,
			{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });
		m_pvulkandevice->flushCommandBuffer(cmdBuffer, m_vkqueue);
	}

	void raytracing_application::deleteStorageImage()
	{
		vkDestroyImageView(m_pvulkandevice->logicalDevice, storageImage.view, nullptr);
		vkDestroyImage(m_pvulkandevice->logicalDevice, storageImage.image, nullptr);
		vkFreeMemory(m_pvulkandevice->logicalDevice, storageImage.memory, nullptr);
	}

	void raytracing_application::prepare()
	{
		base_application_with_swap_chain::prepare();
		// Get properties and features
		rayTracingPipelineProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
		VkPhysicalDeviceProperties2 deviceProperties2{};
		deviceProperties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
		deviceProperties2.pNext = &rayTracingPipelineProperties;
		vkGetPhysicalDeviceProperties2(m_vkphysicaldevice, &deviceProperties2);
		accelerationStructureFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
		VkPhysicalDeviceFeatures2 deviceFeatures2{};
		deviceFeatures2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
		deviceFeatures2.pNext = &accelerationStructureFeatures;
		vkGetPhysicalDeviceFeatures2(m_vkphysicaldevice, &deviceFeatures2);
		// Get the function pointers required for ray tracing
		vkGetBufferDeviceAddressKHR = reinterpret_cast<PFN_vkGetBufferDeviceAddressKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkGetBufferDeviceAddressKHR"_ansi));
		vkCmdBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkCmdBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkCmdBuildAccelerationStructuresKHR"_ansi));
		vkBuildAccelerationStructuresKHR = reinterpret_cast<PFN_vkBuildAccelerationStructuresKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkBuildAccelerationStructuresKHR"_ansi));
		vkCreateAccelerationStructureKHR = reinterpret_cast<PFN_vkCreateAccelerationStructureKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkCreateAccelerationStructureKHR"_ansi));
		vkDestroyAccelerationStructureKHR = reinterpret_cast<PFN_vkDestroyAccelerationStructureKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkDestroyAccelerationStructureKHR"_ansi));
		vkGetAccelerationStructureBuildSizesKHR = reinterpret_cast<PFN_vkGetAccelerationStructureBuildSizesKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkGetAccelerationStructureBuildSizesKHR"_ansi));
		vkGetAccelerationStructureDeviceAddressKHR = reinterpret_cast<PFN_vkGetAccelerationStructureDeviceAddressKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkGetAccelerationStructureDeviceAddressKHR"_ansi));
		vkCmdTraceRaysKHR = reinterpret_cast<PFN_vkCmdTraceRaysKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkCmdTraceRaysKHR"_ansi));
		vkGetRayTracingShaderGroupHandlesKHR = reinterpret_cast<PFN_vkGetRayTracingShaderGroupHandlesKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkGetRayTracingShaderGroupHandlesKHR"_ansi));
		vkCreateRayTracingPipelinesKHR = reinterpret_cast<PFN_vkCreateRayTracingPipelinesKHR>(vkGetDeviceProcAddr(m_vkdevice, "vkCreateRayTracingPipelinesKHR"_ansi));
	}

	VkStridedDeviceAddressRegionKHR raytracing_application::getSbtEntryStridedDeviceAddressRegion(VkBuffer buffer, uint32_t handleCount)
	{
		const uint32_t handleSizeAligned = vks::tools::alignedSize(rayTracingPipelineProperties.shaderGroupHandleSize, rayTracingPipelineProperties.shaderGroupHandleAlignment);
		VkStridedDeviceAddressRegionKHR stridedDeviceAddressRegionKHR{};
		stridedDeviceAddressRegionKHR.deviceAddress = getBufferDeviceAddress(buffer);
		stridedDeviceAddressRegionKHR.stride = handleSizeAligned;
		stridedDeviceAddressRegionKHR.size = handleCount * handleSizeAligned;
		return stridedDeviceAddressRegionKHR;
	}

	void raytracing_application::createShaderBindingTable(ShaderBindingTable& shaderBindingTable, uint32_t handleCount)
	{
		// Create buffer to hold all shader handles for the SBT
		VK_CHECK_RESULT(m_pvulkandevice->createBuffer(
			VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			&shaderBindingTable,
			rayTracingPipelineProperties.shaderGroupHandleSize * handleCount));
		// Get the strided address to be used when dispatching the rays
		shaderBindingTable.stridedDeviceAddressRegion = getSbtEntryStridedDeviceAddressRegion(shaderBindingTable.m_vkbuffer, handleCount);
		// Map persistent 
		shaderBindingTable.map();
	}

	void raytracing_application::drawUI(VkCommandBuffer commandBuffer, VkFramebuffer framebuffer)
	{
		VkClearValue clearValues[2];
		clearValues[0].color = m_vkclearcolorvalueDefault;
		clearValues[1].depthStencil = { 1.0f, 0 };

		VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
		renderPassBeginInfo.renderPass = m_vkrenderpass;
		renderPassBeginInfo.renderArea.offset.x = 0;
		renderPassBeginInfo.renderArea.offset.y = 0;
		renderPassBeginInfo.renderArea.extent.width = m_iWidth;
		renderPassBeginInfo.renderArea.extent.height = m_iHeight;
		renderPassBeginInfo.clearValueCount = 2;
		renderPassBeginInfo.pClearValues = clearValues;
		renderPassBeginInfo.framebuffer = framebuffer;

		vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);
		base_application_with_swap_chain::drawUI(commandBuffer);
		vkCmdEndRenderPass(commandBuffer);
	}


} // namespace vulkan1


