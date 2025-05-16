/*
* Vulkan Example - Offscreen rendering using a separate framebuffer
*
* Copyright (C) 2016-2024 Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "framework.h"
#include "acme/_operating_system.h"
#include "offscreen.h"


VulkanExample::VulkanExample()
   {
      m_strTitle = "Offscreen rendering"_ansi;
      timerSpeed *= 0.25f;
      camera.type = Camera::CameraType::lookat;
      camera.setPosition(glm::vec3(0.0f, 1.0f, -6.0f));
      camera.setRotation(glm::vec3(-2.5f, 0.0f, 0.0f));
      camera.setRotationSpeed(0.5f);
      camera.setPerspective(60.0f, (float)width / (float)height, 0.1f, 256.0f);
      // The scene shader uses a clipping plane, so this feature has to be enabled
      enabledFeatures.shaderClipDistance = VK_TRUE;
   }

VulkanExample::~VulkanExample()
   {
      if (device) {
         // Frame buffer

         // Color attachment
         vkDestroyImageView(device, offscreenPass.color.view, nullptr);
         vkDestroyImage(device, offscreenPass.color.image, nullptr);
         vkFreeMemory(device, offscreenPass.color.mem, nullptr);

         // Depth attachment
         vkDestroyImageView(device, offscreenPass.depth.view, nullptr);
         vkDestroyImage(device, offscreenPass.depth.image, nullptr);
         vkFreeMemory(device, offscreenPass.depth.mem, nullptr);

         vkDestroyRenderPass(device, offscreenPass.renderPass, nullptr);
         vkDestroySampler(device, offscreenPass.sampler, nullptr);
         vkDestroyFramebuffer(device, offscreenPass.frameBuffer, nullptr);

         vkDestroyPipeline(device, pipelines.debug, nullptr);
         vkDestroyPipeline(device, pipelines.shaded, nullptr);
         vkDestroyPipeline(device, pipelines.shadedOffscreen, nullptr);
         vkDestroyPipeline(device, pipelines.mirror, nullptr);

         vkDestroyPipelineLayout(device, pipelineLayouts.textured, nullptr);
         vkDestroyPipelineLayout(device, pipelineLayouts.shaded, nullptr);

         vkDestroyDescriptorSetLayout(device, descriptorSetLayouts.shaded, nullptr);
         vkDestroyDescriptorSetLayout(device, descriptorSetLayouts.textured, nullptr);

         // Uniform buffers
         uniformBuffers.vsShared.destroy();
         uniformBuffers.vsMirror.destroy();
         uniformBuffers.vsOffScreen.destroy();
      }
   }

   // Setup the offscreen framebuffer for rendering the mirrored scene
   // The color attachment of this framebuffer will then be used to sample from in the fragment shader of the final pass
   void VulkanExample::prepareOffscreen()
   {
      offscreenPass.width = FB_DIM;
      offscreenPass.height = FB_DIM;

      // Find a suitable depth format
      VkFormat fbDepthFormat;
      VkBool32 validDepthFormat = vks::tools::getSupportedDepthFormat(physicalDevice, &fbDepthFormat);
      assert(validDepthFormat);

      // Color attachment
      VkImageCreateInfo image = vks::initializers::imageCreateInfo();
      image.imageType = VK_IMAGE_TYPE_2D;
      image.format = FB_COLOR_FORMAT;
      image.extent.width = offscreenPass.width;
      image.extent.height = offscreenPass.height;
      image.extent.depth = 1;
      image.mipLevels = 1;
      image.arrayLayers = 1;
      image.samples = VK_SAMPLE_COUNT_1_BIT;
      image.tiling = VK_IMAGE_TILING_OPTIMAL;
      // We will sample directly from the color attachment
      image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

      VkMemoryAllocateInfo memAlloc = vks::initializers::memoryAllocateInfo();
      VkMemoryRequirements memReqs;

      VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &offscreenPass.color.image));
      vkGetImageMemoryRequirements(device, offscreenPass.color.image, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &offscreenPass.color.mem));
      VK_CHECK_RESULT(vkBindImageMemory(device, offscreenPass.color.image, offscreenPass.color.mem, 0));

      VkImageViewCreateInfo colorImageView = vks::initializers::imageViewCreateInfo();
      colorImageView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      colorImageView.format = FB_COLOR_FORMAT;
      colorImageView.subresourceRange = {};
      colorImageView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      colorImageView.subresourceRange.baseMipLevel = 0;
      colorImageView.subresourceRange.levelCount = 1;
      colorImageView.subresourceRange.baseArrayLayer = 0;
      colorImageView.subresourceRange.layerCount = 1;
      colorImageView.image = offscreenPass.color.image;
      VK_CHECK_RESULT(vkCreateImageView(device, &colorImageView, nullptr, &offscreenPass.color.view));

      // Create sampler to sample from the attachment in the fragment shader
      VkSamplerCreateInfo samplerInfo = vks::initializers::samplerCreateInfo();
      samplerInfo.magFilter = VK_FILTER_LINEAR;
      samplerInfo.minFilter = VK_FILTER_LINEAR;
      samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
      samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
      samplerInfo.addressModeV = samplerInfo.addressModeU;
      samplerInfo.addressModeW = samplerInfo.addressModeU;
      samplerInfo.mipLodBias = 0.0f;
      samplerInfo.maxAnisotropy = 1.0f;
      samplerInfo.minLod = 0.0f;
      samplerInfo.maxLod = 1.0f;
      samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
      VK_CHECK_RESULT(vkCreateSampler(device, &samplerInfo, nullptr, &offscreenPass.sampler));

      // Depth stencil attachment
      image.format = fbDepthFormat;
      image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

      VK_CHECK_RESULT(vkCreateImage(device, &image, nullptr, &offscreenPass.depth.image));
      vkGetImageMemoryRequirements(device, offscreenPass.depth.image, &memReqs);
      memAlloc.allocationSize = memReqs.size;
      memAlloc.memoryTypeIndex = m_pvulkandevice->getMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(device, &memAlloc, nullptr, &offscreenPass.depth.mem));
      VK_CHECK_RESULT(vkBindImageMemory(device, offscreenPass.depth.image, offscreenPass.depth.mem, 0));

      VkImageViewCreateInfo depthStencilView = vks::initializers::imageViewCreateInfo();
      depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
      depthStencilView.format = fbDepthFormat;
      depthStencilView.flags = 0;
      depthStencilView.subresourceRange = {};
      depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
      if (fbDepthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
         depthStencilView.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
      }
      depthStencilView.subresourceRange.baseMipLevel = 0;
      depthStencilView.subresourceRange.levelCount = 1;
      depthStencilView.subresourceRange.baseArrayLayer = 0;
      depthStencilView.subresourceRange.layerCount = 1;
      depthStencilView.image = offscreenPass.depth.image;
      VK_CHECK_RESULT(vkCreateImageView(device, &depthStencilView, nullptr, &offscreenPass.depth.view));

      // Create a separate render pass for the offscreen rendering as it may differ from the one used for scene rendering

      std::array<VkAttachmentDescription, 2> attchmentDescriptions = {};
      // Color attachment
      attchmentDescriptions[0].format = FB_COLOR_FORMAT;
      attchmentDescriptions[0].samples = VK_SAMPLE_COUNT_1_BIT;
      attchmentDescriptions[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attchmentDescriptions[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
      attchmentDescriptions[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      attchmentDescriptions[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attchmentDescriptions[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      attchmentDescriptions[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      // Depth attachment
      attchmentDescriptions[1].format = fbDepthFormat;
      attchmentDescriptions[1].samples = VK_SAMPLE_COUNT_1_BIT;
      attchmentDescriptions[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
      attchmentDescriptions[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attchmentDescriptions[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
      attchmentDescriptions[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
      attchmentDescriptions[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      attchmentDescriptions[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

      VkAttachmentReference colorReference = { 0, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
      VkAttachmentReference depthReference = { 1, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

      VkSubpassDescription subpassDescription = {};
      subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
      subpassDescription.colorAttachmentCount = 1;
      subpassDescription.pColorAttachments = &colorReference;
      subpassDescription.pDepthStencilAttachment = &depthReference;

      // Use subpass dependencies for layout transitions
      std::array<VkSubpassDependency, 2> dependencies;

      dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
      dependencies[0].dstSubpass = 0;
      dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      dependencies[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      dependencies[0].srcAccessMask = VK_ACCESS_NONE_KHR;
      dependencies[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

      dependencies[1].srcSubpass = 0;
      dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
      dependencies[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
      dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
      dependencies[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
      dependencies[1].dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
      dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

      // Create the actual renderpass
      VkRenderPassCreateInfo renderPassInfo = {};
      renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
      renderPassInfo.attachmentCount = static_cast<uint32_t>(attchmentDescriptions.size());
      renderPassInfo.pAttachments = attchmentDescriptions.data();
      renderPassInfo.subpassCount = 1;
      renderPassInfo.pSubpasses = &subpassDescription;
      renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
      renderPassInfo.pDependencies = dependencies.data();

      VK_CHECK_RESULT(vkCreateRenderPass(device, &renderPassInfo, nullptr, &offscreenPass.renderPass));

      VkImageView attachments[2];
      attachments[0] = offscreenPass.color.view;
      attachments[1] = offscreenPass.depth.view;

      VkFramebufferCreateInfo fbufCreateInfo = vks::initializers::framebufferCreateInfo();
      fbufCreateInfo.renderPass = offscreenPass.renderPass;
      fbufCreateInfo.attachmentCount = 2;
      fbufCreateInfo.pAttachments = attachments;
      fbufCreateInfo.width = offscreenPass.width;
      fbufCreateInfo.height = offscreenPass.height;
      fbufCreateInfo.layers = 1;

      VK_CHECK_RESULT(vkCreateFramebuffer(device, &fbufCreateInfo, nullptr, &offscreenPass.frameBuffer));

      // Fill a descriptor for later use in a descriptor set
      offscreenPass.descriptor.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      offscreenPass.descriptor.imageView = offscreenPass.color.view;
      offscreenPass.descriptor.sampler = offscreenPass.sampler;
   }

   void VulkanExample::buildCommandBuffers()
   {
      VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();

      for (int32_t i = 0; i < m_drawCmdBuffers.size(); ++i)
      {
         VK_CHECK_RESULT(vkBeginCommandBuffer(m_drawCmdBuffers[i], &cmdBufInfo));

         /*
            First render pass: Offscreen rendering
         */
         {
            VkClearValue clearValues[2];
            clearValues[0].color = { { 0.0f, 0.0f, 0.0f, 0.0f } };
            clearValues[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = offscreenPass.renderPass;
            renderPassBeginInfo.framebuffer = offscreenPass.frameBuffer;
            renderPassBeginInfo.renderArea.extent.width = offscreenPass.width;
            renderPassBeginInfo.renderArea.extent.height = offscreenPass.height;
            renderPassBeginInfo.clearValueCount = 2;
            renderPassBeginInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport = vks::initializers::viewport((float)offscreenPass.width, (float)offscreenPass.height, 0.0f, 1.0f);
            vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

            VkRect2D scissor = vks::initializers::rect2D(offscreenPass.width, offscreenPass.height, 0, 0);
            vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

            // Mirrored scene
            vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayouts.shaded, 0, 1, &descriptorSets.offscreen, 0, NULL);
            vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.shadedOffscreen);
            models.example.draw(m_drawCmdBuffers[i]);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
         }

         /*
            Note: Explicit synchronization is not required between the render pass, as this is done implicit via sub pass dependencies
         */

         /*
            Second render pass: Scene rendering with applied radial blur
         */
         {
            VkClearValue clearValues[2];
            clearValues[0].color = defaultClearColor;
            clearValues[1].depthStencil = { 1.0f, 0 };

            VkRenderPassBeginInfo renderPassBeginInfo = vks::initializers::renderPassBeginInfo();
            renderPassBeginInfo.renderPass = renderPass;
            renderPassBeginInfo.framebuffer = frameBuffers[i];
            renderPassBeginInfo.renderArea.extent.width = width;
            renderPassBeginInfo.renderArea.extent.height = height;
            renderPassBeginInfo.clearValueCount = 2;
            renderPassBeginInfo.pClearValues = clearValues;

            vkCmdBeginRenderPass(m_drawCmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

            VkViewport viewport = vks::initializers::viewport((float)width, (float)height, 0.0f, 1.0f);
            vkCmdSetViewport(m_drawCmdBuffers[i], 0, 1, &viewport);

            VkRect2D scissor = vks::initializers::rect2D(width, height, 0, 0);
            vkCmdSetScissor(m_drawCmdBuffers[i], 0, 1, &scissor);

            if (debugDisplay)
            {
               // Display the offscreen render target
               vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayouts.textured, 0, 1, &descriptorSets.mirror, 0, nullptr);
               vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.debug);
               vkCmdDraw(m_drawCmdBuffers[i], 3, 1, 0, 0);
            }
            else {
               // Render the scene
               // Reflection plane
               vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayouts.textured, 0, 1, &descriptorSets.mirror, 0, nullptr);
               vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.mirror);
               models.plane.draw(m_drawCmdBuffers[i]);
               // Model
               vkCmdBindDescriptorSets(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayouts.shaded, 0, 1, &descriptorSets.model, 0, nullptr);
               vkCmdBindPipeline(m_drawCmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipelines.shaded);
               models.example.draw(m_drawCmdBuffers[i]);
            }

            drawUI(m_drawCmdBuffers[i]);

            vkCmdEndRenderPass(m_drawCmdBuffers[i]);
         }

         VK_CHECK_RESULT(vkEndCommandBuffer(m_drawCmdBuffers[i]));
      }
   }

   void VulkanExample::loadAssets()
   {
      const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;

      ::vks::VulkanDevice * pdevice = m_pvulkandevice;
      ::file::path pathFolder(getAssetPath().c_str());
      auto pathPlaneGltf = pathFolder / "models/plane.gltf"_ansi;
      auto pathDragonGltf = pathFolder / "models/chinesedragon.gltf"_ansi;
      models.plane.loadFromFile(pathPlaneGltf, pdevice, queue, glTFLoadingFlags);
      models.example.loadFromFile(pathDragonGltf, pdevice, queue, glTFLoadingFlags);
   }

   void VulkanExample::setupDescriptors()
   {
      // Pool
      std::vector<VkDescriptorPoolSize> poolSizes = {
         vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 6),
         vks::initializers::descriptorPoolSize(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 8)
      };
      VkDescriptorPoolCreateInfo descriptorPoolInfo = vks::initializers::descriptorPoolCreateInfo(poolSizes, 5);
      VK_CHECK_RESULT(vkCreateDescriptorPool(device, &descriptorPoolInfo, nullptr, &descriptorPool));

      // Layout
      std::vector<VkDescriptorSetLayoutBinding> setLayoutBindings;
      VkDescriptorSetLayoutCreateInfo descriptorLayoutInfo;

      // Shaded layout
      setLayoutBindings = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
      };
      descriptorLayoutInfo = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
      VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayoutInfo, nullptr, &descriptorSetLayouts.shaded));

      // Textured layouts
      setLayoutBindings = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, 0),
         // Binding 1 : Fragment shader image sampler
         vks::initializers::descriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 1)
      };
      descriptorLayoutInfo = vks::initializers::descriptorSetLayoutCreateInfo(setLayoutBindings);
      VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device, &descriptorLayoutInfo, nullptr, &descriptorSetLayouts.textured));

      // Sets
      // Mirror plane descriptor set
      VkDescriptorSetAllocateInfo allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.textured, 1);
      VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets.mirror));
      std::vector<VkWriteDescriptorSet> writeDescriptorSets = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::writeDescriptorSet(descriptorSets.mirror, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.vsMirror.descriptor),
         // Binding 1 : Fragment shader texture sampler
         vks::initializers::writeDescriptorSet(descriptorSets.mirror, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1, &offscreenPass.descriptor),
      };
      vkUpdateDescriptorSets(device, static_cast<uint32_t>(writeDescriptorSets.size()), writeDescriptorSets.data(), 0, nullptr);

      // Shaded descriptor sets
      allocInfo = vks::initializers::descriptorSetAllocateInfo(descriptorPool, &descriptorSetLayouts.shaded, 1);
      // Model
      VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets.model));
      std::vector<VkWriteDescriptorSet> modelWriteDescriptorSets = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::writeDescriptorSet(descriptorSets.model, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.vsShared.descriptor)
      };
      vkUpdateDescriptorSets(device, static_cast<uint32_t>(modelWriteDescriptorSets.size()), modelWriteDescriptorSets.data(), 0, nullptr);

      // Offscreen
      VK_CHECK_RESULT(vkAllocateDescriptorSets(device, &allocInfo, &descriptorSets.offscreen));
      std::vector<VkWriteDescriptorSet> offScreenWriteDescriptorSets = {
         // Binding 0 : Vertex shader uniform buffer
         vks::initializers::writeDescriptorSet(descriptorSets.offscreen, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 0, &uniformBuffers.vsOffScreen.descriptor)
      };
      vkUpdateDescriptorSets(device, static_cast<uint32_t>(offScreenWriteDescriptorSets.size()), offScreenWriteDescriptorSets.data(), 0, nullptr);

   }

   void VulkanExample::preparePipelines()
   {
      // Layouts
      VkPipelineLayoutCreateInfo pipelineLayoutInfo = vks::initializers::pipelineLayoutCreateInfo(&descriptorSetLayouts.shaded, 1);
      VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayouts.shaded));

      pipelineLayoutInfo = vks::initializers::pipelineLayoutCreateInfo(&descriptorSetLayouts.textured, 1);
      VK_CHECK_RESULT(vkCreatePipelineLayout(device, &pipelineLayoutInfo, nullptr, &pipelineLayouts.textured));

      // Pipelines
      VkPipelineInputAssemblyStateCreateInfo inputAssemblyState = vks::initializers::pipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
      VkPipelineRasterizationStateCreateInfo rasterizationState = vks::initializers::pipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_COUNTER_CLOCKWISE, 0);
      VkPipelineColorBlendAttachmentState blendAttachmentState = vks::initializers::pipelineColorBlendAttachmentState(0xf, VK_FALSE);
      VkPipelineColorBlendStateCreateInfo colorBlendState = vks::initializers::pipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
      VkPipelineDepthStencilStateCreateInfo depthStencilState = vks::initializers::pipelineDepthStencilStateCreateInfo(VK_TRUE, VK_TRUE, VK_COMPARE_OP_LESS_OR_EQUAL);
      VkPipelineViewportStateCreateInfo viewportState = vks::initializers::pipelineViewportStateCreateInfo(1, 1, 0);
      VkPipelineMultisampleStateCreateInfo multisampleState = vks::initializers::pipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT, 0);
      std::vector<VkDynamicState> dynamicStateEnables = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
      VkPipelineDynamicStateCreateInfo dynamicState = vks::initializers::pipelineDynamicStateCreateInfo(dynamicStateEnables);
      std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages;

      VkGraphicsPipelineCreateInfo pipelineCI = vks::initializers::pipelineCreateInfo(pipelineLayouts.textured, renderPass, 0);
      pipelineCI.pInputAssemblyState = &inputAssemblyState;
      pipelineCI.pRasterizationState = &rasterizationState;
      pipelineCI.pColorBlendState = &colorBlendState;
      pipelineCI.pMultisampleState = &multisampleState;
      pipelineCI.pViewportState = &viewportState;
      pipelineCI.pDepthStencilState = &depthStencilState;
      pipelineCI.pDynamicState = &dynamicState;
      pipelineCI.stageCount = static_cast<uint32_t>(shaderStages.size());
      pipelineCI.pStages = shaderStages.data();
      pipelineCI.pVertexInputState = vkglTF::Vertex::getPipelineVertexInputState({ vkglTF::VertexComponent::Position, vkglTF::VertexComponent::Color, vkglTF::VertexComponent::Normal });

      rasterizationState.cullMode = VK_CULL_MODE_NONE;

      // Render-target debug display
      shaderStages[0] = loadShader(getShadersPath() + "offscreen/quad.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
      shaderStages[1] = loadShader(getShadersPath() + "offscreen/quad.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.debug));

      // Mirror
      shaderStages[0] = loadShader(getShadersPath() + "offscreen/mirror.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
      shaderStages[1] = loadShader(getShadersPath() + "offscreen/mirror.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.mirror));

      rasterizationState.cullMode = VK_CULL_MODE_BACK_BIT;

      // Phong shading pipelines
      pipelineCI.layout = pipelineLayouts.shaded;
      // Scene
      shaderStages[0] = loadShader(getShadersPath() + "offscreen/phong.vert.spv", VK_SHADER_STAGE_VERTEX_BIT);
      shaderStages[1] = loadShader(getShadersPath() + "offscreen/phong.frag.spv", VK_SHADER_STAGE_FRAGMENT_BIT);
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.shaded));
      // Offscreen
      // Flip cull mode
      rasterizationState.cullMode = VK_CULL_MODE_FRONT_BIT;
      pipelineCI.renderPass = offscreenPass.renderPass;
      VK_CHECK_RESULT(vkCreateGraphicsPipelines(device, pipelineCache, 1, &pipelineCI, nullptr, &pipelines.shadedOffscreen));

   }

   // Prepare and initialize uniform buffer containing shader uniforms
   void VulkanExample::prepareUniformBuffers()
   {
      // Mesh vertex shader uniform buffer block
      VK_CHECK_RESULT(m_pvulkandevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &uniformBuffers.vsShared, sizeof(UniformData)));
      // Mirror plane vertex shader uniform buffer block
      VK_CHECK_RESULT(m_pvulkandevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &uniformBuffers.vsMirror, sizeof(UniformData)));
      // Offscreen vertex shader uniform buffer block
      VK_CHECK_RESULT(m_pvulkandevice->createBuffer(VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &uniformBuffers.vsOffScreen, sizeof(UniformData)));
      // Map persistent
      VK_CHECK_RESULT(uniformBuffers.vsShared.map());
      VK_CHECK_RESULT(uniformBuffers.vsMirror.map());
      VK_CHECK_RESULT(uniformBuffers.vsOffScreen.map());

      updateUniformBuffers();
      updateUniformBufferOffscreen();
   }

   void VulkanExample::updateUniformBuffers()
   {
      uniformData.projection = camera.matrices.perspective;
      uniformData.view = camera.matrices.view;

      // Model
      uniformData.model = glm::mat4(1.0f);
      uniformData.model = glm::rotate(uniformData.model, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      uniformData.model = glm::translate(uniformData.model, modelPosition);
      memcpy(uniformBuffers.vsShared.mapped, &uniformData, sizeof(UniformData));

      // Mirror
      uniformData.model = glm::mat4(1.0f);
      memcpy(uniformBuffers.vsMirror.mapped, &uniformData, sizeof(UniformData));
   }

   void VulkanExample::updateUniformBufferOffscreen()
   {
      uniformData.projection = camera.matrices.perspective;
      uniformData.view = camera.matrices.view;
      uniformData.model = glm::mat4(1.0f);
      uniformData.model = glm::rotate(uniformData.model, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
      uniformData.model = glm::scale(uniformData.model, glm::vec3(1.0f, -1.0f, 1.0f));
      uniformData.model = glm::translate(uniformData.model, modelPosition);
      memcpy(uniformBuffers.vsOffScreen.mapped, &uniformData, sizeof(UniformData));
   }

   void VulkanExample::prepare()
   {
      VulkanExampleBase::prepare();
      loadAssets();
      prepareOffscreen();
      prepareUniformBuffers();
      setupDescriptors();
      preparePipelines();
      buildCommandBuffers();
      prepared = true;
   }

   void VulkanExample::draw()
   {
      VulkanExampleBase::prepareFrame();
      submitInfo.commandBufferCount = 1;
      submitInfo.pCommandBuffers = &m_drawCmdBuffers[currentBuffer];
      VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));
      VulkanExampleBase::submitFrame();
   }

   void VulkanExample::render()
   {
      if (!prepared)
         return;
      draw();
      if (!paused || camera.updated)
      {
         if (!paused) {
            modelRotation.y += frameTimer * 10.0f;
         }
         updateUniformBuffers();
         updateUniformBufferOffscreen();
      }
   }

   void VulkanExample::OnUpdateUIOverlay(vks::UIOverlay * overlay)
   {
      if (overlay->header("Settings"_ansi)) {
         if (overlay->checkBox("Display render target"_ansi, &debugDisplay)) {
            buildCommandBuffers();
         }
      }
   }
///};

VulkanExample *vulkanExample;																		
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)						
{																									
	if (vulkanExample != NULL)																		
	{																								
		vulkanExample->handleMessages(hWnd, uMsg, wParam, lParam);									
	}																								
	return (DefWindowProc(hWnd, uMsg, wParam, lParam));												
}																									
int run_vulkan_example()									
{																									
	vulkanExample = ___new VulkanExample();															
	vulkanExample->initVulkan();																	
	vulkanExample->setupWindow(::GetModuleHandleW(L"app_core_vulken.dll"), WndProc);
	vulkanExample->prepare();																		
	vulkanExample->renderLoop();																	
	delete(vulkanExample);																			
	return 0;																						
}


///*
// * Vulkan entry points
// *
// * Platform specific macros for the example main entry points
// *
// * Copyright (C) 2024 by Sascha Willems - www.saschawillems.de
// *
// * This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
// */
//
//#if defined(_WIN32)
// /*
//  * Windows
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)						\
//{																									\
//	if (vulkanExample != NULL)																		\
//	{																								\
//		vulkanExample->handleMessages(hWnd, uMsg, wParam, lParam);									\
//	}																								\
//	return (DefWindowProc(hWnd, uMsg, wParam, lParam));												\
//}																									\
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)									\
//{																									\
//	for (int32_t i = 0; i < __argc; i++) { VulkanExample::args.push_back(__argv[i]); };  			\
//	vulkanExample = ___new VulkanExample();															\
//	vulkanExample->initVulkan();																	\
//	vulkanExample->setupWindow(hInstance, WndProc);													\
//	vulkanExample->prepare();																		\
//	vulkanExample->renderLoop();																	\
//	delete(vulkanExample);																			\
//	return 0;																						\
//}
//
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
// /*
//  * Android
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//void android_main(android_app* state)																\
//{																									\
//	vulkanExample = ___new VulkanExample();															\
//	state->userData = vulkanExample;																\
//	state->onAppCmd = VulkanExample::handleAppCommand;												\
//	state->onInputEvent = VulkanExample::handleAppInput;											\
//	androidApp = state;																				\
//	vks::android::getDeviceConfig();																\
//	vulkanExample->renderLoop();																	\
//	delete(vulkanExample);																			\
//}

//#elif defined(_DIRECT2DISPLAY)
// /*
//  * Direct-to-display
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//static void handleEvent()                                											\
//{																									\
//}																									\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
//	vulkanExample = ___new VulkanExample();															\
//	vulkanExample->initVulkan();																	\
//	vulkanExample->prepare();																		\
//	vulkanExample->renderLoop();																	\
//	delete(vulkanExample);																			\
//	return 0;																						\
//}

//#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
// /*
//  * Direct FB
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//static void handleEvent(const DFBWindowEvent *happening)												\
//{																									\
//	if (vulkanExample != NULL)																		\
//	{																								\
//		vulkanExample->handleEvent(happening);															\
//	}																								\
//}																									\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
//	vulkanExample = ___new VulkanExample();															\
//	vulkanExample->initVulkan();																	\
//	vulkanExample->setupWindow();					 												\
//	vulkanExample->prepare();																		\
//	vulkanExample->renderLoop();																	\
//	delete(vulkanExample);																			\
//	return 0;																						\
//}

//#elif (defined(VK_USE_PLATFORM_WAYLAND_KHR) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
// /*
//  * Wayland / headless
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
//	vulkanExample = ___new VulkanExample();															\
//	vulkanExample->initVulkan();																	\
//	vulkanExample->setupWindow();					 												\
//	vulkanExample->prepare();																		\
//	vulkanExample->renderLoop();																	\
//	delete(vulkanExample);																			\
//	return 0;																						\
//}

//#elif defined(VK_USE_PLATFORM_XCB_KHR)
// /*
//  * X11 Xcb
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//static void handleEvent(const xcb_generic_event_t *happening)											\
//{																									\
//	if (vulkanExample != NULL)																		\
//	{																								\
//		vulkanExample->handleEvent(happening);															\
//	}																								\
//}																									\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };  				\
//	vulkanExample = ___new VulkanExample();															\
//	vulkanExample->initVulkan();																	\
//	vulkanExample->setupWindow();					 												\
//	vulkanExample->prepare();																		\
//	vulkanExample->renderLoop();																	\
//	delete(vulkanExample);																			\
//	return 0;																						\
//}

//#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
// /*
//  * iOS and macOS (using MoltenVK)
//  */
//#if defined(VK_EXAMPLE_XCODE_GENERATED)
//#define VULKAN_EXAMPLE_MAIN()																		\
//VulkanExample *vulkanExample;																		\
//int main(const int argc, const char *argv[])														\
//{																									\
//	@autoreleasepool																				\
//	{																								\
//		for (size_t i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };				\
//		vulkanExample = ___new VulkanExample();														\
//		vulkanExample->initVulkan();																\
//		vulkanExample->setupWindow(nullptr);														\
//		vulkanExample->prepare();																	\
//		vulkanExample->renderLoop();																\
//		delete(vulkanExample);																		\
//	}																								\
//	return 0;																						\
//}
//#else
//#define VULKAN_EXAMPLE_MAIN()
//#endif
//
//#elif defined(VK_USE_PLATFORM_SCREEN_QNX)
// /*
//  * QNX Screen
//  */
//#define VULKAN_EXAMPLE_MAIN()																		
//VulkanExample *vulkanExample;																		
//int main(const int argc, const char *argv[])														
//{																									
//	for (int i = 0; i < argc; i++) { VulkanExample::args.push_back(argv[i]); };						
//	vulkanExample = ___new VulkanExample();															
//	vulkanExample->initVulkan();																	
//	vulkanExample->setupWindow();																	
//	vulkanExample->prepare();																		
//	vulkanExample->renderLoop();																	
//	delete(vulkanExample);																			
//	return 0;																						
//}
//
//
//
