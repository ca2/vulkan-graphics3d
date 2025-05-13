#pragma once
#include <vector>
#include <memory>
#include <deque>
#include <functional>

//#include "vma/vk_mem_alloc.h"
#include "vulkan-land/landen/vulkan/VulkanDevice.h"
#include "vulkan-land/landen/vulkan/vulkan_example_base_no_swap_chain.h"
#include "VK_abstraction/vk_descriptors.h"
#include "VK_abstraction/vk_gameObject.h"
#include "vulkan-land/landen/vulkan/vk_container.h"
#include "Renderer/vk_renderer.h"

namespace vkc 
{


   //class VulkanExample20 : public VulkanExampleBaseNoSwapChain
   //{
   //public:
   //   bool debugDisplay = false;

   //   struct {
   //      vkglTF::Model example;
   //      vkglTF::Model plane;
   //   } models;

   //   struct {
   //      vks::Buffer vsShared;
   //      vks::Buffer vsMirror;
   //      vks::Buffer vsOffScreen;
   //   } uniformBuffers;

   //   struct UniformData {
   //      glm::mat4 projection;
   //      glm::mat4 view;
   //      glm::mat4 model;
   //      glm::vec4 lightPos = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
   //   } uniformData;

   //   struct {
   //      VkPipeline debug{ VK_NULL_HANDLE };
   //      VkPipeline shaded{ VK_NULL_HANDLE };
   //      VkPipeline shadedOffscreen{ VK_NULL_HANDLE };
   //      VkPipeline mirror{ VK_NULL_HANDLE };
   //   } pipelines;

   //   struct {
   //      VkPipelineLayout textured{ VK_NULL_HANDLE };
   //      VkPipelineLayout shaded{ VK_NULL_HANDLE };
   //   } pipelineLayouts;

   //   struct {
   //      VkDescriptorSet offscreen{ VK_NULL_HANDLE };
   //      VkDescriptorSet mirror{ VK_NULL_HANDLE };
   //      VkDescriptorSet model{ VK_NULL_HANDLE };
   //   } descriptorSets;

   //   struct {
   //      VkDescriptorSetLayout textured{ VK_NULL_HANDLE };
   //      VkDescriptorSetLayout shaded{ VK_NULL_HANDLE };
   //   } descriptorSetLayouts;

   //   // Framebuffer for offscreen rendering
   //   struct FrameBufferAttachment {
   //      VkImage image;
   //      VkDeviceMemory mem;
   //      VkImageView view;
   //   };
   //   struct OffscreenPass {
   //      int32_t width, height;
   //      VkFramebuffer frameBuffer;
   //      FrameBufferAttachment color, depth;
   //      VkRenderPass renderPass;
   //      VkSampler sampler;
   //      VkDescriptorImageInfo descriptor;
   //   } offscreenPass{};

   //   glm::vec3 modelPosition = glm::vec3(0.0f, -1.0f, 0.0f);
   //   glm::vec3 modelRotation = glm::vec3(0.0f);

   //   VulkanExample20();
   //   ~VulkanExample20();

   //   // Setup the offscreen framebuffer for rendering the mirrored scene
   //   // The color attachment of this framebuffer will then be used to sample from in the fragment shader of the final pass
   //   void prepareOffscreen();

   //   void buildCommandBuffers();

   //   void loadAssets();

   //   void setupDescriptors();

   //   void preparePipelines();
   //   // Prepare and initialize uniform buffer containing shader uniforms
   //   void prepareUniformBuffers();

   //   void updateUniformBuffers();
   //   void updateUniformBufferOffscreen();

   //   void prepare();

   //   void submitWork(VkCommandBuffer cmdBuffer, VkQueue queue);

   //   void draw(const ::function < void(void*, int, int, int)>& callback);

   //   virtual void render(const ::function < void(void*, int, int, int)>& callback);

   //   virtual void OnUpdateUIOverlay(vks::UIOverlay* overlay);

   //   void sample(const ::function < void(void*, int, int, int)>& callback);

   //   void render_loop(const ::function < void(void*, int, int, int)>& callback);

   //};

	class Application :
      public VulkanExampleBaseNoSwapChain 
   {
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		Application();
		~Application();

		Application(const Application&) = delete;
		Application& operator=(const Application&) = delete;

      void initialize_application(::vkc::VkContainer * pvkcontainer);

      void run_application() override;

	public:


      void resize(int cx, int cy) override;

		void loadGameObjects();

		// Private Members
		//VkWindow _window{ WIDTH, HEIGHT, "Vulkan window" };
      ::pointer < ::vkc::VkContainer>     m_pvkcontainer;
		::pointer < VkcDevice >             m_pvkcdevice;
		::pointer < Renderer >              m_prenderer;
 

		::pointer <VkcDescriptorPool> m_pglobalpool;
		
		VkcGameObject::Map m_gameObjects;

	};

}




