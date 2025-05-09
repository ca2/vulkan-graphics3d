#pragma once

#include "VK_abstraction/vk_camera.h"
#include "VK_abstraction/vk_device.h"
#include "VK_abstraction/vk_frameInfo.h"
#include "VK_abstraction/vk_gameObject.h"
#include "VK_abstraction/vk_pipeline.h"

// std
#include <memory>
#include <vector>

namespace vkc {
    class PointLightSystem {
    public:
        PointLightSystem(
            VkcDevice *pvkcdevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~PointLightSystem();

        PointLightSystem(const PointLightSystem&) = delete;
        PointLightSystem& operator=(const PointLightSystem&) = delete;

      
        void render(FrameInfo& frameInfo);
        void update(FrameInfo& framInfo, GlobalUbo& ubo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        ::pointer < VkcDevice > m_pvkcdevice;

        ::pointer<VkcPipeline> vkcPipeline;
        VkPipelineLayout pipelineLayout;
    };
}  // namespace lve