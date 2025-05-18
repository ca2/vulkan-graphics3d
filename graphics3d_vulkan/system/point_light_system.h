#pragma once

#include "app-cube/cube/camera.h"
#include "context.h"
#include "frame_info.h"
#include "scene_object.h"
#include "pipeline.h"

// std
#include <memory>
#include <vector>

namespace graphics3d_vulkan 
{
    class point_light_system
    {
    public:


        point_light_system(
            context *pdevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
        ~point_light_system();

        point_light_system(const point_light_system&) = delete;
        point_light_system& operator=(const point_light_system&) = delete;

      
        void render(FrameInfo& frameInfo);
        void update(FrameInfo& framInfo, GlobalUbo& ubo);

    private:
        void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
        void createPipeline(VkRenderPass renderPass);

        ::pointer < context > m_pcontext;

        ::pointer < pipeline> m_ppipeline;

        VkPipelineLayout pipelineLayout;

    };
}  // namespace lve