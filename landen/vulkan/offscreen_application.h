#pragma once


#include "base_application_no_swap_chain.h"
#include "VulkanglTFModel.h"

// Offscreen frame buffer properties
#define FB_DIM 512
#define FB_COLOR_FORMAT VK_FORMAT_R8G8B8A8_UNORM

namespace vulkan
{


   class offscreen_application : 
      virtual public base_application_no_swap_chain
   {
   public:


      bool debugDisplay = false;

      struct {
         vkglTF::Model example;
         vkglTF::Model plane;
      } models;

      struct {
         vulkan::buffer vsShared;
         vulkan::buffer vsMirror;
         vulkan::buffer vsOffScreen;
      } uniformBuffers;

      struct UniformData {
         glm::mat4 projection;
         glm::mat4 view;
         glm::mat4 model;
         glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
      } uniformData;

      struct {
         VkPipeline debug{ VK_NULL_HANDLE };
         VkPipeline shaded{ VK_NULL_HANDLE };
         VkPipeline shadedOffscreen{ VK_NULL_HANDLE };
         VkPipeline mirror{ VK_NULL_HANDLE };
      } pipelines;

      struct {
         VkPipelineLayout textured{ VK_NULL_HANDLE };
         VkPipelineLayout shaded{ VK_NULL_HANDLE };
      } pipelineLayouts;

      struct {
         VkDescriptorSet offscreen{ VK_NULL_HANDLE };
         VkDescriptorSet mirror{ VK_NULL_HANDLE };
         VkDescriptorSet model{ VK_NULL_HANDLE };
      } descriptorSets;

      struct {
         VkDescriptorSetLayout textured{ VK_NULL_HANDLE };
         VkDescriptorSetLayout shaded{ VK_NULL_HANDLE };
      } descriptorSetLayouts;

      // Framebuffer for offscreen rendering
      struct FrameBufferAttachment {
         VkImage image;
         VkDeviceMemory mem;
         VkImageView view;
      };
      struct OffscreenPass {
         int32_t width, height;
         VkFramebuffer frameBuffer;
         FrameBufferAttachment color, depth;
         VkRenderPass renderPass;
         VkSampler sampler;
         VkDescriptorImageInfo descriptor;
      } offscreenPass{};

      glm::vec3 modelPosition = glm::vec3(0.0f, -1.0f, 0.0f);
      glm::vec3 modelRotation = glm::vec3(0.0f);

      offscreen_application();
      ~offscreen_application();


      void run_application() override;

      // Setup the offscreen framebuffer for rendering the mirrored scene
      // The color attachment of this framebuffer will then be used to sample from in the fragment shader of the final pass
      void prepareOffscreen();

      void buildCommandBuffers();

      void loadAssets();

      void setupDescriptors();

      void preparePipelines();
      // Prepare and initialize uniform buffer containing shader uniforms
      void prepareUniformBuffers();

      void updateUniformBuffers();
      void updateUniformBufferOffscreen();

      void prepare();

      void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);

      void draw(const ::function < void(void*, int, int, int)>& callback);

      virtual void render(const ::function < void(void*, int, int, int)>& callback);

      virtual void OnUpdateUIOverlay(vulkan::ui_overlay* overlay);

      void sample(const ::function < void(void*, int, int, int)>& callback);

      void render_loop(const ::function < void(void*, int, int, int)>& callback);

   };


} // namespace vulkan



