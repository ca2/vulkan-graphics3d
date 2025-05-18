#pragma once

#include "app-cube/cube/camera.h"
#include "application_object.h"

// lib
//#include "_vulkan/vulkan.h>

namespace graphics3d_vulkan {


#define MAX_LIGHTS 10

	struct PointLight {
		glm::vec4 position{};  // ignore w
		glm::vec4 color{};     // w is intensity
	};


	struct GlobalUbo {
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::mat4 inverseView{ 1.f };
		glm::vec4 ambientLightColor{ 1.f, 1.f, 1.f, .02f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct FrameInfo {
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		::cube::camera &camera;
		VkDescriptorSet globalDescriptorSet;
		::cube::application_object::map& gameObjects;
	};
}