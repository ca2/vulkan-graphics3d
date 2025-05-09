/*
* Android Vulkan function pointer loader
*
* Copyright (C) 2016-2023 by Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "framework.h"
#include "VulkanAndroid.h"

#if defined(__ANDROID__)
	#include <android/log.h>
	#include <dlfcn.h>
	#include <android/native_window_jni.h>

android_app* androidApp;

PFN_vkCreateInstance vkCreateInstance;
PFN_vkGetDeviceProcAddr vkGetDeviceProcAddr;
PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
PFN_vkCreateDevice vkCreateDevice;
PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices;
PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties;
PFN_vkGetPhysicalDeviceProperties2 vkGetPhysicalDeviceProperties2;
PFN_vkEnumerateDeviceExtensionProperties vkEnumerateDeviceExtensionProperties;
PFN_vkEnumerateDeviceLayerProperties vkEnumerateDeviceLayerProperties;
PFN_vkGetPhysicalDeviceFormatProperties vkGetPhysicalDeviceFormatProperties;
PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures;
PFN_vkGetPhysicalDeviceFeatures2 vkGetPhysicalDeviceFeatures2;
PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties;
PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties;
PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
PFN_vkCmdPipelineBarrier vkCmdPipelineBarrier;
PFN_vkCreateShaderModule vkCreateShaderModule;
PFN_vkCreateBuffer vkCreateBuffer;
PFN_vkGetBufferMemoryRequirements vkGetBufferMemoryRequirements;
PFN_vkMapMemory vkMapMemory;
PFN_vkUnmapMemory vkUnmapMemory;
PFN_vkFlushMappedMemoryRanges vkFlushMappedMemoryRanges;
PFN_vkInvalidateMappedMemoryRanges vkInvalidateMappedMemoryRanges;
PFN_vkBindBufferMemory vkBindBufferMemory;
PFN_vkDestroyBuffer vkDestroyBuffer;
PFN_vkAllocateMemory vkAllocateMemory;
PFN_vkBindImageMemory vkBindImageMemory;
PFN_vkGetImageSubresourceLayout vkGetImageSubresourceLayout;
PFN_vkCmdCopyBuffer vkCmdCopyBuffer;
PFN_vkCmdCopyBufferToImage vkCmdCopyBufferToImage;
PFN_vkCmdCopyImage vkCmdCopyImage;
PFN_vkCmdBlitImage vkCmdBlitImage;
PFN_vkCmdClearAttachments vkCmdClearAttachments;
PFN_vkCreateSampler vkCreateSampler;
PFN_vkDestroySampler vkDestroySampler;
PFN_vkDestroyImage vkDestroyImage;
PFN_vkFreeMemory vkFreeMemory;
PFN_vkCreateRenderPass vkCreateRenderPass;
PFN_vkCmdBeginRenderPass vkCmdBeginRenderPass;
PFN_vkCmdEndRenderPass vkCmdEndRenderPass;
PFN_vkCmdNextSubpass vkCmdNextSubpass;
PFN_vkCmdExecuteCommands vkCmdExecuteCommands;
PFN_vkCmdClearColorImage vkCmdClearColorImage;
PFN_vkCreateImage vkCreateImage;
PFN_vkGetImageMemoryRequirements vkGetImageMemoryRequirements;
PFN_vkCreateImageView vkCreateImageView;
PFN_vkDestroyImageView vkDestroyImageView;
PFN_vkCreateSemaphore vkCreateSemaphore;
PFN_vkDestroySemaphore vkDestroySemaphore;
PFN_vkCreateFence vkCreateFence;
PFN_vkDestroyFence vkDestroyFence;
PFN_vkWaitForFences vkWaitForFences;
PFN_vkResetFences vkResetFences;
PFN_vkResetDescriptorPool vkResetDescriptorPool;
PFN_vkCreateCommandPool vkCreateCommandPool;
PFN_vkDestroyCommandPool vkDestroyCommandPool;
PFN_vkAllocateCommandBuffers vkAllocateCommandBuffers;
PFN_vkBeginCommandBuffer vkBeginCommandBuffer;
PFN_vkEndCommandBuffer vkEndCommandBuffer;
PFN_vkGetDeviceQueue vkGetDeviceQueue;
PFN_vkQueueSubmit vkQueueSubmit;
PFN_vkQueueWaitIdle vkQueueWaitIdle;
PFN_vkDeviceWaitIdle vkDeviceWaitIdle;
PFN_vkCreateFramebuffer vkCreateFramebuffer;
PFN_vkCreatePipelineCache vkCreatePipelineCache;
PFN_vkCreatePipelineLayout vkCreatePipelineLayout;
PFN_vkCreateGraphicsPipelines vkCreateGraphicsPipelines;
PFN_vkCreateComputePipelines vkCreateComputePipelines;
PFN_vkCreateDescriptorPool vkCreateDescriptorPool;
PFN_vkCreateDescriptorSetLayout vkCreateDescriptorSetLayout;
PFN_vkAllocateDescriptorSets vkAllocateDescriptorSets;
PFN_vkUpdateDescriptorSets vkUpdateDescriptorSets;
PFN_vkCmdBindDescriptorSets vkCmdBindDescriptorSets;
PFN_vkCmdBindPipeline vkCmdBindPipeline;
PFN_vkCmdBindVertexBuffers vkCmdBindVertexBuffers;
PFN_vkCmdBindIndexBuffer vkCmdBindIndexBuffer;
PFN_vkCmdSetViewport vkCmdSetViewport;
PFN_vkCmdSetScissor vkCmdSetScissor;
PFN_vkCmdSetLineWidth vkCmdSetLineWidth;
PFN_vkCmdSetDepthBias vkCmdSetDepthBias;
PFN_vkCmdPushConstants vkCmdPushConstants;
PFN_vkCmdDrawIndexed vkCmdDrawIndexed;
PFN_vkCmdDraw vkCmdDraw;
PFN_vkCmdDrawIndexedIndirect vkCmdDrawIndexedIndirect;
PFN_vkCmdDrawIndirect vkCmdDrawIndirect;
PFN_vkCmdDispatch vkCmdDispatch;
PFN_vkDestroyPipeline vkDestroyPipeline;
PFN_vkDestroyPipelineLayout vkDestroyPipelineLayout;
PFN_vkDestroyDescriptorSetLayout vkDestroyDescriptorSetLayout;
PFN_vkDestroyDevice vkDestroyDevice;
PFN_vkDestroyInstance vkDestroyInstance;
PFN_vkDestroyDescriptorPool vkDestroyDescriptorPool;
PFN_vkFreeCommandBuffers vkFreeCommandBuffers;
PFN_vkDestroyRenderPass vkDestroyRenderPass;
PFN_vkDestroyFramebuffer vkDestroyFramebuffer;
PFN_vkDestroyShaderModule vkDestroyShaderModule;
PFN_vkDestroyPipelineCache vkDestroyPipelineCache;
PFN_vkCreateQueryPool vkCreateQueryPool;
PFN_vkDestroyQueryPool vkDestroyQueryPool;
PFN_vkGetQueryPoolResults vkGetQueryPoolResults;
PFN_vkCmdBeginQuery vkCmdBeginQuery;
PFN_vkCmdEndQuery vkCmdEndQuery;
PFN_vkCmdResetQueryPool vkCmdResetQueryPool;
PFN_vkCmdCopyQueryPoolResults vkCmdCopyQueryPoolResults;

PFN_vkCreateAndroidSurfaceKHR vkCreateAndroidSurfaceKHR;
PFN_vkDestroySurfaceKHR vkDestroySurfaceKHR;
PFN_vkCmdFillBuffer vkCmdFillBuffer;

PFN_vkGetPhysicalDeviceSurfaceSupportKHR vkGetPhysicalDeviceSurfaceSupportKHR;
PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR vkGetPhysicalDeviceSurfaceCapabilitiesKHR;
PFN_vkGetPhysicalDeviceSurfaceFormatsKHR vkGetPhysicalDeviceSurfaceFormatsKHR;
PFN_vkGetPhysicalDeviceSurfacePresentModesKHR vkGetPhysicalDeviceSurfacePresentModesKHR;
PFN_vkCreateSwapchainKHR vkCreateSwapchainKHR;
PFN_vkDestroySwapchainKHR vkDestroySwapchainKHR;
PFN_vkGetSwapchainImagesKHR vkGetSwapchainImagesKHR;
PFN_vkAcquireNextImageKHR vkAcquireNextImageKHR;
PFN_vkQueuePresentKHR vkQueuePresentKHR;

PFN_vkResetCommandBuffer vkResetCommandBuffer;

int32_t vks::android::screenDensity;

void *libVulkan;

namespace vks
{
	namespace android
	{
		// Dynamically load Vulkan library and base function pointers
		bool loadVulkanLibrary()
		{
			__android_log_print(ANDROID_LOG_INFO, "vulkanandroid"_ansi, "Loading libvulkan.so...\n"_ansi);

			// Load vulkan library
			libVulkan = dlopen("libvulkan.so"_ansi, RTLD_NOW | RTLD_LOCAL);
			if (!libVulkan)
			{
				__android_log_print(ANDROID_LOG_INFO, "vulkanandroid"_ansi, "Could not load vulkan library : %s!\n"_ansi, dlerror());
				return false;
			}

			// Load base function pointers
			vkEnumerateInstanceExtensionProperties = reinterpret_cast<PFN_vkEnumerateInstanceExtensionProperties>(dlsym(libVulkan, "vkEnumerateInstanceExtensionProperties"_ansi));
			vkEnumerateInstanceLayerProperties = reinterpret_cast<PFN_vkEnumerateInstanceLayerProperties>(dlsym(libVulkan, "vkEnumerateInstanceLayerProperties"_ansi));
			vkCreateInstance = reinterpret_cast<PFN_vkCreateInstance>(dlsym(libVulkan, "vkCreateInstance"_ansi));
			vkGetInstanceProcAddr = reinterpret_cast<PFN_vkGetInstanceProcAddr>(dlsym(libVulkan, "vkGetInstanceProcAddr"_ansi));
			vkGetDeviceProcAddr = reinterpret_cast<PFN_vkGetDeviceProcAddr>(dlsym(libVulkan, "vkGetDeviceProcAddr"_ansi));

			return true;
		}

		// Load instance based Vulkan function pointers
		void loadVulkanFunctions(VkInstance instance)
		{
			__android_log_print(ANDROID_LOG_INFO, "vulkanandroid"_ansi, "Loading instance based function pointers...\n"_ansi);

			vkEnumeratePhysicalDevices = reinterpret_cast<PFN_vkEnumeratePhysicalDevices>(vkGetInstanceProcAddr(instance, "vkEnumeratePhysicalDevices"_ansi));
			vkGetPhysicalDeviceProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties"_ansi));
			vkGetPhysicalDeviceProperties2 = reinterpret_cast<PFN_vkGetPhysicalDeviceProperties2>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceProperties2"_ansi));
			vkEnumerateDeviceLayerProperties = reinterpret_cast<PFN_vkEnumerateDeviceLayerProperties>(vkGetInstanceProcAddr(instance, "vkEnumerateDeviceLayerProperties"_ansi));
			vkEnumerateDeviceExtensionProperties = reinterpret_cast<PFN_vkEnumerateDeviceExtensionProperties>(vkGetInstanceProcAddr(instance, "vkEnumerateDeviceExtensionProperties"_ansi));
			vkGetPhysicalDeviceQueueFamilyProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceQueueFamilyProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceQueueFamilyProperties"_ansi));
			vkGetPhysicalDeviceFeatures = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures"_ansi));
			vkGetPhysicalDeviceFeatures2 = reinterpret_cast<PFN_vkGetPhysicalDeviceFeatures2>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFeatures2"_ansi));
			vkCreateDevice = reinterpret_cast<PFN_vkCreateDevice>(vkGetInstanceProcAddr(instance, "vkCreateDevice"_ansi));
			vkGetPhysicalDeviceFormatProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceFormatProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceFormatProperties"_ansi));
			vkGetPhysicalDeviceMemoryProperties = reinterpret_cast<PFN_vkGetPhysicalDeviceMemoryProperties>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceMemoryProperties"_ansi));

			vkCmdPipelineBarrier = reinterpret_cast<PFN_vkCmdPipelineBarrier>(vkGetInstanceProcAddr(instance, "vkCmdPipelineBarrier"_ansi));
			vkCreateShaderModule = reinterpret_cast<PFN_vkCreateShaderModule>(vkGetInstanceProcAddr(instance, "vkCreateShaderModule"_ansi));

			vkCreateBuffer = reinterpret_cast<PFN_vkCreateBuffer>(vkGetInstanceProcAddr(instance, "vkCreateBuffer"_ansi));
			vkGetBufferMemoryRequirements = reinterpret_cast<PFN_vkGetBufferMemoryRequirements>(vkGetInstanceProcAddr(instance, "vkGetBufferMemoryRequirements"_ansi));
			vkMapMemory = reinterpret_cast<PFN_vkMapMemory>(vkGetInstanceProcAddr(instance, "vkMapMemory"_ansi));
			vkUnmapMemory = reinterpret_cast<PFN_vkUnmapMemory>(vkGetInstanceProcAddr(instance, "vkUnmapMemory"_ansi));
			vkFlushMappedMemoryRanges = reinterpret_cast<PFN_vkFlushMappedMemoryRanges>(vkGetInstanceProcAddr(instance, "vkFlushMappedMemoryRanges"_ansi));
			vkInvalidateMappedMemoryRanges = reinterpret_cast<PFN_vkInvalidateMappedMemoryRanges>(vkGetInstanceProcAddr(instance, "vkInvalidateMappedMemoryRanges"_ansi));
			vkBindBufferMemory = reinterpret_cast<PFN_vkBindBufferMemory>(vkGetInstanceProcAddr(instance, "vkBindBufferMemory"_ansi));
			vkDestroyBuffer = reinterpret_cast<PFN_vkDestroyBuffer>(vkGetInstanceProcAddr(instance, "vkDestroyBuffer"_ansi));

			vkAllocateMemory = reinterpret_cast<PFN_vkAllocateMemory>(vkGetInstanceProcAddr(instance, "vkAllocateMemory"_ansi));
			vkFreeMemory = reinterpret_cast<PFN_vkFreeMemory>(vkGetInstanceProcAddr(instance, "vkFreeMemory"_ansi));
			vkCreateRenderPass = reinterpret_cast<PFN_vkCreateRenderPass>(vkGetInstanceProcAddr(instance, "vkCreateRenderPass"_ansi));
			vkCmdBeginRenderPass = reinterpret_cast<PFN_vkCmdBeginRenderPass>(vkGetInstanceProcAddr(instance, "vkCmdBeginRenderPass"_ansi));
			vkCmdEndRenderPass = reinterpret_cast<PFN_vkCmdEndRenderPass>(vkGetInstanceProcAddr(instance, "vkCmdEndRenderPass"_ansi));
			vkCmdNextSubpass = reinterpret_cast<PFN_vkCmdNextSubpass>(vkGetInstanceProcAddr(instance, "vkCmdNextSubpass"_ansi));
			vkCmdExecuteCommands = reinterpret_cast<PFN_vkCmdExecuteCommands>(vkGetInstanceProcAddr(instance, "vkCmdExecuteCommands"_ansi));
	        vkCmdClearColorImage = reinterpret_cast<PFN_vkCmdClearColorImage>(vkGetInstanceProcAddr(instance, "vkCmdClearColorImage"_ansi));

			vkCreateImage = reinterpret_cast<PFN_vkCreateImage>(vkGetInstanceProcAddr(instance, "vkCreateImage"_ansi));
			vkGetImageMemoryRequirements = reinterpret_cast<PFN_vkGetImageMemoryRequirements>(vkGetInstanceProcAddr(instance, "vkGetImageMemoryRequirements"_ansi));
			vkCreateImageView = reinterpret_cast<PFN_vkCreateImageView>(vkGetInstanceProcAddr(instance, "vkCreateImageView"_ansi));
			vkDestroyImageView = reinterpret_cast<PFN_vkDestroyImageView>(vkGetInstanceProcAddr(instance, "vkDestroyImageView"_ansi));
			vkBindImageMemory = reinterpret_cast<PFN_vkBindImageMemory>(vkGetInstanceProcAddr(instance, "vkBindImageMemory"_ansi));
			vkGetImageSubresourceLayout = reinterpret_cast<PFN_vkGetImageSubresourceLayout>(vkGetInstanceProcAddr(instance, "vkGetImageSubresourceLayout"_ansi));
			vkCmdCopyImage = reinterpret_cast<PFN_vkCmdCopyImage>(vkGetInstanceProcAddr(instance, "vkCmdCopyImage"_ansi));
			vkCmdBlitImage = reinterpret_cast<PFN_vkCmdBlitImage>(vkGetInstanceProcAddr(instance, "vkCmdBlitImage"_ansi));
			vkDestroyImage = reinterpret_cast<PFN_vkDestroyImage>(vkGetInstanceProcAddr(instance, "vkDestroyImage"_ansi));

			vkCmdClearAttachments = reinterpret_cast<PFN_vkCmdClearAttachments>(vkGetInstanceProcAddr(instance, "vkCmdClearAttachments"_ansi));

			vkCmdCopyBuffer = reinterpret_cast<PFN_vkCmdCopyBuffer>(vkGetInstanceProcAddr(instance, "vkCmdCopyBuffer"_ansi));
			vkCmdCopyBufferToImage = reinterpret_cast<PFN_vkCmdCopyBufferToImage>(vkGetInstanceProcAddr(instance, "vkCmdCopyBufferToImage"_ansi));

			vkCreateSampler = reinterpret_cast<PFN_vkCreateSampler>(vkGetInstanceProcAddr(instance, "vkCreateSampler"_ansi));
			vkDestroySampler = reinterpret_cast<PFN_vkDestroySampler>(vkGetInstanceProcAddr(instance, "vkDestroySampler"_ansi));;

			vkCreateSemaphore = reinterpret_cast<PFN_vkCreateSemaphore>(vkGetInstanceProcAddr(instance, "vkCreateSemaphore"_ansi));
			vkDestroySemaphore = reinterpret_cast<PFN_vkDestroySemaphore>(vkGetInstanceProcAddr(instance, "vkDestroySemaphore"_ansi));

			vkCreateFence = reinterpret_cast<PFN_vkCreateFence>(vkGetInstanceProcAddr(instance, "vkCreateFence"_ansi));
			vkDestroyFence = reinterpret_cast<PFN_vkDestroyFence>(vkGetInstanceProcAddr(instance, "vkDestroyFence"_ansi));
			vkWaitForFences = reinterpret_cast<PFN_vkWaitForFences>(vkGetInstanceProcAddr(instance, "vkWaitForFences"_ansi));
			vkResetFences = reinterpret_cast<PFN_vkResetFences>(vkGetInstanceProcAddr(instance, "vkResetFences"_ansi));;
	        vkResetDescriptorPool = reinterpret_cast<PFN_vkResetDescriptorPool>(vkGetInstanceProcAddr(instance, "vkResetDescriptorPool"_ansi));

			vkCreateCommandPool = reinterpret_cast<PFN_vkCreateCommandPool>(vkGetInstanceProcAddr(instance, "vkCreateCommandPool"_ansi));
			vkDestroyCommandPool = reinterpret_cast<PFN_vkDestroyCommandPool>(vkGetInstanceProcAddr(instance, "vkDestroyCommandPool"_ansi));;

			vkAllocateCommandBuffers = reinterpret_cast<PFN_vkAllocateCommandBuffers>(vkGetInstanceProcAddr(instance, "vkAllocateCommandBuffers"_ansi));
			vkBeginCommandBuffer = reinterpret_cast<PFN_vkBeginCommandBuffer>(vkGetInstanceProcAddr(instance, "vkBeginCommandBuffer"_ansi));
			vkEndCommandBuffer = reinterpret_cast<PFN_vkEndCommandBuffer>(vkGetInstanceProcAddr(instance, "vkEndCommandBuffer"_ansi));

			vkGetDeviceQueue = reinterpret_cast<PFN_vkGetDeviceQueue>(vkGetInstanceProcAddr(instance, "vkGetDeviceQueue"_ansi));
			vkQueueSubmit = reinterpret_cast<PFN_vkQueueSubmit>(vkGetInstanceProcAddr(instance, "vkQueueSubmit"_ansi));
			vkQueueWaitIdle = reinterpret_cast<PFN_vkQueueWaitIdle>(vkGetInstanceProcAddr(instance, "vkQueueWaitIdle"_ansi));

			vkDeviceWaitIdle = reinterpret_cast<PFN_vkDeviceWaitIdle>(vkGetInstanceProcAddr(instance, "vkDeviceWaitIdle"_ansi));

			vkCreateFramebuffer = reinterpret_cast<PFN_vkCreateFramebuffer>(vkGetInstanceProcAddr(instance, "vkCreateFramebuffer"_ansi));

			vkCreatePipelineCache = reinterpret_cast<PFN_vkCreatePipelineCache>(vkGetInstanceProcAddr(instance, "vkCreatePipelineCache"_ansi));
			vkCreatePipelineLayout = reinterpret_cast<PFN_vkCreatePipelineLayout>(vkGetInstanceProcAddr(instance, "vkCreatePipelineLayout"_ansi));
			vkCreateGraphicsPipelines = reinterpret_cast<PFN_vkCreateGraphicsPipelines>(vkGetInstanceProcAddr(instance, "vkCreateGraphicsPipelines"_ansi));
			vkCreateComputePipelines = reinterpret_cast<PFN_vkCreateComputePipelines>(vkGetInstanceProcAddr(instance, "vkCreateComputePipelines"_ansi));

			vkCreateDescriptorPool = reinterpret_cast<PFN_vkCreateDescriptorPool>(vkGetInstanceProcAddr(instance, "vkCreateDescriptorPool"_ansi));
			vkCreateDescriptorSetLayout = reinterpret_cast<PFN_vkCreateDescriptorSetLayout>(vkGetInstanceProcAddr(instance, "vkCreateDescriptorSetLayout"_ansi));

			vkAllocateDescriptorSets = reinterpret_cast<PFN_vkAllocateDescriptorSets>(vkGetInstanceProcAddr(instance, "vkAllocateDescriptorSets"_ansi));
			vkUpdateDescriptorSets = reinterpret_cast<PFN_vkUpdateDescriptorSets>(vkGetInstanceProcAddr(instance, "vkUpdateDescriptorSets"_ansi));

			vkCmdBindDescriptorSets = reinterpret_cast<PFN_vkCmdBindDescriptorSets>(vkGetInstanceProcAddr(instance, "vkCmdBindDescriptorSets"_ansi));
			vkCmdBindPipeline = reinterpret_cast<PFN_vkCmdBindPipeline>(vkGetInstanceProcAddr(instance, "vkCmdBindPipeline"_ansi));
			vkCmdBindVertexBuffers = reinterpret_cast<PFN_vkCmdBindVertexBuffers>(vkGetInstanceProcAddr(instance, "vkCmdBindVertexBuffers"_ansi));
			vkCmdBindIndexBuffer = reinterpret_cast<PFN_vkCmdBindIndexBuffer>(vkGetInstanceProcAddr(instance, "vkCmdBindIndexBuffer"_ansi));

			vkCmdSetViewport = reinterpret_cast<PFN_vkCmdSetViewport>(vkGetInstanceProcAddr(instance, "vkCmdSetViewport"_ansi));
			vkCmdSetScissor = reinterpret_cast<PFN_vkCmdSetScissor>(vkGetInstanceProcAddr(instance, "vkCmdSetScissor"_ansi));
			vkCmdSetLineWidth = reinterpret_cast<PFN_vkCmdSetLineWidth>(vkGetInstanceProcAddr(instance, "vkCmdSetLineWidth"_ansi));
			vkCmdSetDepthBias = reinterpret_cast<PFN_vkCmdSetDepthBias>(vkGetInstanceProcAddr(instance, "vkCmdSetDepthBias"_ansi));
			vkCmdPushConstants = reinterpret_cast<PFN_vkCmdPushConstants>(vkGetInstanceProcAddr(instance, "vkCmdPushConstants"_ansi));;

			vkCmdDrawIndexed = reinterpret_cast<PFN_vkCmdDrawIndexed>(vkGetInstanceProcAddr(instance, "vkCmdDrawIndexed"_ansi));
			vkCmdDraw = reinterpret_cast<PFN_vkCmdDraw>(vkGetInstanceProcAddr(instance, "vkCmdDraw"_ansi));
			vkCmdDrawIndexedIndirect = reinterpret_cast<PFN_vkCmdDrawIndexedIndirect>(vkGetInstanceProcAddr(instance, "vkCmdDrawIndexedIndirect"_ansi));
			vkCmdDrawIndirect = reinterpret_cast<PFN_vkCmdDrawIndirect>(vkGetInstanceProcAddr(instance, "vkCmdDrawIndirect"_ansi));
			vkCmdDispatch = reinterpret_cast<PFN_vkCmdDispatch>(vkGetInstanceProcAddr(instance, "vkCmdDispatch"_ansi));

			vkDestroyPipeline = reinterpret_cast<PFN_vkDestroyPipeline>(vkGetInstanceProcAddr(instance, "vkDestroyPipeline"_ansi));
			vkDestroyPipelineLayout = reinterpret_cast<PFN_vkDestroyPipelineLayout>(vkGetInstanceProcAddr(instance, "vkDestroyPipelineLayout"_ansi));;
			vkDestroyDescriptorSetLayout = reinterpret_cast<PFN_vkDestroyDescriptorSetLayout>(vkGetInstanceProcAddr(instance, "vkDestroyDescriptorSetLayout"_ansi));
			vkDestroyDevice = reinterpret_cast<PFN_vkDestroyDevice>(vkGetInstanceProcAddr(instance, "vkDestroyDevice"_ansi));
			vkDestroyInstance = reinterpret_cast<PFN_vkDestroyInstance>(vkGetInstanceProcAddr(instance, "vkDestroyInstance"_ansi));
			vkDestroyDescriptorPool = reinterpret_cast<PFN_vkDestroyDescriptorPool>(vkGetInstanceProcAddr(instance, "vkDestroyDescriptorPool"_ansi));
			vkFreeCommandBuffers = reinterpret_cast<PFN_vkFreeCommandBuffers>(vkGetInstanceProcAddr(instance, "vkFreeCommandBuffers"_ansi));
			vkDestroyRenderPass = reinterpret_cast<PFN_vkDestroyRenderPass>(vkGetInstanceProcAddr(instance, "vkDestroyRenderPass"_ansi));
			vkDestroyFramebuffer = reinterpret_cast<PFN_vkDestroyFramebuffer>(vkGetInstanceProcAddr(instance, "vkDestroyFramebuffer"_ansi));
			vkDestroyShaderModule = reinterpret_cast<PFN_vkDestroyShaderModule>(vkGetInstanceProcAddr(instance, "vkDestroyShaderModule"_ansi));
			vkDestroyPipelineCache = reinterpret_cast<PFN_vkDestroyPipelineCache>(vkGetInstanceProcAddr(instance, "vkDestroyPipelineCache"_ansi));

			vkCreateQueryPool = reinterpret_cast<PFN_vkCreateQueryPool>(vkGetInstanceProcAddr(instance, "vkCreateQueryPool"_ansi));
			vkDestroyQueryPool = reinterpret_cast<PFN_vkDestroyQueryPool>(vkGetInstanceProcAddr(instance, "vkDestroyQueryPool"_ansi));
			vkGetQueryPoolResults = reinterpret_cast<PFN_vkGetQueryPoolResults>(vkGetInstanceProcAddr(instance, "vkGetQueryPoolResults"_ansi));

			vkCmdBeginQuery = reinterpret_cast<PFN_vkCmdBeginQuery>(vkGetInstanceProcAddr(instance, "vkCmdBeginQuery"_ansi));
			vkCmdEndQuery = reinterpret_cast<PFN_vkCmdEndQuery>(vkGetInstanceProcAddr(instance, "vkCmdEndQuery"_ansi));
			vkCmdResetQueryPool = reinterpret_cast<PFN_vkCmdResetQueryPool>(vkGetInstanceProcAddr(instance, "vkCmdResetQueryPool"_ansi));
			vkCmdCopyQueryPoolResults = reinterpret_cast<PFN_vkCmdCopyQueryPoolResults>(vkGetInstanceProcAddr(instance, "vkCmdCopyQueryPoolResults"_ansi));

			vkCreateAndroidSurfaceKHR = reinterpret_cast<PFN_vkCreateAndroidSurfaceKHR>(vkGetInstanceProcAddr(instance, "vkCreateAndroidSurfaceKHR"_ansi));
			vkDestroySurfaceKHR = reinterpret_cast<PFN_vkDestroySurfaceKHR>(vkGetInstanceProcAddr(instance, "vkDestroySurfaceKHR"_ansi));

			vkCmdFillBuffer = reinterpret_cast<PFN_vkCmdFillBuffer>(vkGetInstanceProcAddr(instance, "vkCmdFillBuffer"_ansi));

            vkGetPhysicalDeviceSurfaceSupportKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceSupportKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceSupportKHR"_ansi));
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceCapabilitiesKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR"_ansi));
            vkGetPhysicalDeviceSurfaceFormatsKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfaceFormatsKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfaceFormatsKHR"_ansi));
            vkGetPhysicalDeviceSurfacePresentModesKHR = reinterpret_cast<PFN_vkGetPhysicalDeviceSurfacePresentModesKHR>(vkGetInstanceProcAddr(instance, "vkGetPhysicalDeviceSurfacePresentModesKHR"_ansi));
            vkCreateSwapchainKHR = reinterpret_cast<PFN_vkCreateSwapchainKHR>(vkGetInstanceProcAddr(instance, "vkCreateSwapchainKHR"_ansi));
            vkDestroySwapchainKHR = reinterpret_cast<PFN_vkDestroySwapchainKHR>(vkGetInstanceProcAddr(instance, "vkDestroySwapchainKHR"_ansi));
            vkGetSwapchainImagesKHR = reinterpret_cast<PFN_vkGetSwapchainImagesKHR>(vkGetInstanceProcAddr(instance, "vkGetSwapchainImagesKHR"_ansi));
            vkAcquireNextImageKHR = reinterpret_cast<PFN_vkAcquireNextImageKHR>(vkGetInstanceProcAddr(instance, "vkAcquireNextImageKHR"_ansi));
            vkQueuePresentKHR = reinterpret_cast<PFN_vkQueuePresentKHR>(vkGetInstanceProcAddr(instance, "vkQueuePresentKHR"_ansi));

            vkResetCommandBuffer = reinterpret_cast<PFN_vkResetCommandBuffer>(vkGetInstanceProcAddr(instance, "vkResetCommandBuffer"_ansi));
		}

		void freeVulkanLibrary()
		{
			dlclose(libVulkan);
		}

		void getDeviceConfig()
		{
			// Screen density
			AConfiguration* config = AConfiguration_new();
			AConfiguration_fromAssetManager(config, androidApp->activity->assetManager);
			vks::android::screenDensity = AConfiguration_getDensity(config);
			AConfiguration_delete(config);
		}

		// Displays a native alert dialog using JNI
		void showAlert(const char* message) {
			JNIEnv* jni;
			androidApp->activity->vm->AttachCurrentThread(&jni, NULL);

			jstring jmessage = jni->NewStringUTF(message);

			jclass clazz = jni->GetObjectClass(androidApp->activity->clazz);
			// Signature has to match java implementation (arguments)
			jmethodID methodID = jni->GetMethodID(clazz, "showAlert"_ansi, "(Ljava/lang/String;)V"_ansi);
			jni->CallVoidMethod(androidApp->activity->clazz, methodID, jmessage);
			jni->DeleteLocalRef(jmessage);

			androidApp->activity->vm->DetachCurrentThread();
			return;
		}
	}
}

#endif
