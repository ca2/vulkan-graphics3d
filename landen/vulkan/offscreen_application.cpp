/*
* Vulkan Example - Offscreen rendering using a separate framebuffer
*
* Copyright (C) 2016-2024 Sascha Willems - www.saschawillems.de
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
*/
#include "framework.h"
#include "acme/_operating_system.h"
#include "offscreen_application.h"
#include "vulkan/vk_container.h"


namespace vulkan
{


offscreen_application::offscreen_application()
{
   m_colorformatFB = FB_COLOR_FORMAT;
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

offscreen_application::~offscreen_application()
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
void offscreen_application::prepareOffscreen()
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

void offscreen_application::buildCommandBuffers()
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

void offscreen_application::loadAssets()
{
   const uint32_t glTFLoadingFlags = vkglTF::FileLoadingFlags::PreTransformVertices | vkglTF::FileLoadingFlags::PreMultiplyVertexColors | vkglTF::FileLoadingFlags::FlipY;

   ::vks::VulkanDevice * pdevice = m_pvulkandevice;
   ::file::path pathFolder(getAssetPath().c_str());
   auto pathPlaneGltf = pathFolder / "models/plane.gltf"_ansi;
   auto pathDragonGltf = pathFolder / "models/chinesedragon.gltf"_ansi;
   //auto pathDragonGltf = pathFolder / "models/plants.gltf"_ansi;
   //auto pathDragonGltf = pathFolder / "models/cube.gltf"_ansi;
   models.plane.loadFromFile(pathPlaneGltf, pdevice, queue, glTFLoadingFlags);
   models.example.loadFromFile(pathDragonGltf, pdevice, queue, glTFLoadingFlags);
}

void offscreen_application::setupDescriptors()
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

void offscreen_application::preparePipelines()
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
void offscreen_application::prepareUniformBuffers()
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

void offscreen_application::updateUniformBuffers()
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

void offscreen_application::updateUniformBufferOffscreen()
{
   uniformData.projection = camera.matrices.perspective;
   uniformData.view = camera.matrices.view;
   uniformData.model = glm::mat4(1.0f);
   uniformData.model = glm::rotate(uniformData.model, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
   uniformData.model = glm::scale(uniformData.model, glm::vec3(1.0f, -1.0f, 1.0f));
   uniformData.model = glm::translate(uniformData.model, modelPosition);
   memcpy(uniformBuffers.vsOffScreen.mapped, &uniformData, sizeof(UniformData));
}

void offscreen_application::prepare()
{
   base_application_no_swap_chain::prepare();
   loadAssets();
   prepareOffscreen();
   prepareUniformBuffers();
   setupDescriptors();
   preparePipelines();
   buildCommandBuffers();
   prepared = true;
}

void offscreen_application::draw(const ::function < void(void *, int, int, int)> & callback)
{
   //base_application_no_swap_chain::prepareFrame();
   //submitInfo.commandBufferCount = 1;
   //submitInfo.pCommandBuffers = &m_drawCmdBuffers[currentBuffer];
   //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));

   submitWork(m_drawCmdBuffers[currentBuffer], queue);
   //base_application_no_swap_chain::submitFrame();
   vkQueueWaitIdle(queue);
   sample(callback);
}


void offscreen_application::submitWork(VkCommandBuffer cmdBuffer, VkQueue queue)
{
   VkSubmitInfo submitInfo = vks::initializers::submitInfo();
   submitInfo.commandBufferCount = 1;
   submitInfo.pCommandBuffers = &cmdBuffer;
   //m_submitInfo.commandBufferCount = 1;
   //m_submitInfo.pCommandBuffers = &cmdBuffer;
   VkFenceCreateInfo fenceInfo = vks::initializers::fenceCreateInfo();
   VkFence fence;
   VK_CHECK_RESULT(vkCreateFence(m_pvulkandevice->logicalDevice, &fenceInfo, nullptr, &fence));
   VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, fence));
   VK_CHECK_RESULT(vkWaitForFences(m_pvulkandevice->logicalDevice, 1, &fence, VK_TRUE, UINT64_MAX));
   vkDestroyFence(m_pvulkandevice->logicalDevice, fence, nullptr);
}


void offscreen_application::render(const ::function < void(void *, int, int, int)> & callback)
{
   if (!prepared)
      return;
   draw(callback);
   if (!paused || camera.updated)
   {
      if (!paused) {
         modelRotation.y += frameTimer * 10.0f;
      }
      updateUniformBuffers();
      updateUniformBufferOffscreen();
   }
}

void offscreen_application::OnUpdateUIOverlay(vks::UIOverlay * overlay)
{
   if (overlay->header("Settings"_ansi)) {
      if (overlay->checkBox("Display render target"_ansi, &debugDisplay)) {
         buildCommandBuffers();
      }
   }
}
///};

void offscreen_application::sample(const ::function < void(void *, int, int, int)> & callback)
{


   /*
      Copy framebuffer image to host visible image
   */
   const char * imagedata;
   {
      // Create the linear tiled destination image to copy to and to read the memory from
      VkImageCreateInfo imgCreateInfo(vks::initializers::imageCreateInfo());
      imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imgCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      imgCreateInfo.extent.width = width;
      imgCreateInfo.extent.height = height;
      imgCreateInfo.extent.depth = 1;
      imgCreateInfo.arrayLayers = 1;
      imgCreateInfo.mipLevels = 1;
      imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      // Create the image
      VkImage dstImage;
      VK_CHECK_RESULT(vkCreateImage(device, &imgCreateInfo, nullptr, &dstImage));
      // Create memory to back up the image
      VkMemoryRequirements memRequirements;
      VkMemoryAllocateInfo memAllocInfo(vks::initializers::memoryAllocateInfo());
      VkDeviceMemory dstImageMemory;
      vkGetImageMemoryRequirements(device, dstImage, &memRequirements);
      memAllocInfo.allocationSize = memRequirements.size;
      // Memory must be host visible to copy from
      memAllocInfo.memoryTypeIndex = m_pvulkandevice->getMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(device, &memAllocInfo, nullptr, &dstImageMemory));
      VK_CHECK_RESULT(vkBindImageMemory(device, dstImage, dstImageMemory, 0));

      // Do the actual blit from the offscreen image to our host visible destination image
      VkCommandBufferAllocateInfo cmdBufAllocateInfo = vks::initializers::commandBufferAllocateInfo(cmdPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
      VkCommandBuffer copyCmd;
      VK_CHECK_RESULT(vkAllocateCommandBuffers(device, &cmdBufAllocateInfo, &copyCmd));
      VkCommandBufferBeginInfo cmdBufInfo = vks::initializers::commandBufferBeginInfo();
      VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

      // Transition destination image to transfer destination layout
      vks::tools::insertImageMemoryBarrier(
         copyCmd,
         dstImage,
         0,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_IMAGE_LAYOUT_UNDEFINED,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      // colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

      VkImageCopy imageCopyRegion{};
      imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.srcSubresource.layerCount = 1;
      imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      imageCopyRegion.dstSubresource.layerCount = 1;
      imageCopyRegion.extent.width = width;
      imageCopyRegion.extent.height = height;
      imageCopyRegion.extent.depth = 1;

      vkCmdCopyImage(
         copyCmd,
         //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         m_colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         1,
         &imageCopyRegion);

      // Transition destination image to general layout, which is the required layout for mapping the image memory later on
      vks::tools::insertImageMemoryBarrier(
         copyCmd,
         dstImage,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_ACCESS_MEMORY_READ_BIT,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_IMAGE_LAYOUT_GENERAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      submitWork(copyCmd, queue);

      //submitInfo.commandBufferCount = 1;
      //submitInfo.pCommandBuffers = &copyCmd;
      //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));


      // Get layout of the image (including row pitch)
      VkImageSubresource subResource{};
      subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      VkSubresourceLayout subResourceLayout;

      vkGetImageSubresourceLayout(device, dstImage, &subResource, &subResourceLayout);

      // Map image memory so we can start copying from it
      vkMapMemory(device, dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void **)&imagedata);
      imagedata += subResourceLayout.offset;

      /*
         Save host visible framebuffer image to disk (ppm format)
      */

      //::memory mem;

      //mem.set_size(m_width * m_height * 4);

      //std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      if (callback)
      {
         callback((void *)imagedata, width, height, subResourceLayout.rowPitch);

      }

      //// ppm binary pixel data
      //for (int32_t y = 0; y < m_height; y++) {
      //   unsigned int * row = (unsigned int *)imagedata;
      //   for (int32_t x = 0; x < m_width; x++) {
      //      //               if (colorSwizzle) {
      //         file.write((char *)row + 2, 1);
      //         file.write((char *)row + 1, 1);
      //         file.write((char *)row, 1);
      //      }
      //      //else {
      //      //   file.write((char *)row, 3);
      //      //}
      //      row++;
      //   }
      //   imagedata += subResourceLayout.rowPitch;
      //}


//         callback

//#if defined (VK_USE_PLATFORM_ANDROID_KHR)
//         const char * filename = strcat(getenv("EXTERNAL_STORAGE"_ansi), "/headless.ppm"_ansi);
//#else
//         const char * filename = "headless.ppm"_ansi;
//#endif
//         std::ofstream file(filename, std::ios::out | std::ios::binary);
//
//         // ppm header
//         file << "P6\n"_ansi << m_width << "\n"_ansi << m_height << "\n"_ansi << 255 << "\n"_ansi;

         //// If source is BGR (destination is always RGB) and we can't use blit (which does automatic conversion), we'hi have to manually swizzle color components
         //// Check if source is BGR and needs swizzle
         //std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
         //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());

         //// ppm binary pixel data
         //for (int32_t y = 0; y < m_height; y++) {
         //   unsigned int * row = (unsigned int *)imagedata;
         //   for (int32_t x = 0; x < m_width; x++) {
         //      if (colorSwizzle) {
         //         file.write((char *)row + 2, 1);
         //         file.write((char *)row + 1, 1);
         //         file.write((char *)row, 1);
         //      }
         //      else {
         //         file.write((char *)row, 3);
         //      }
         //      row++;
         //   }
         //   imagedata += subResourceLayout.rowPitch;
         //}
         //file.close();

         //LOG("Framebuffer image saved to %s\n"_ansi, filename);

         // Clean up resources
      vkUnmapMemory(device, dstImageMemory);
      vkFreeMemory(device, dstImageMemory, nullptr);
      vkDestroyImage(device, dstImage, nullptr);
   }

   vkQueueWaitIdle(queue);

}



void offscreen_application::render_loop(const ::function < void(void *, int, int, int)> & callback)
{


   //MSG msg;
   //bool quitMessageReceived = false;
   //while (!quitMessageReceived) {
   //   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
   //      TranslateMessage(&msg);
   //      DispatchMessage(&msg);
   //      if (msg.message == WM_QUIT) {
   //         quitMessageReceived = true;
   //         break;
   //      }
   //   }
   //   if (prepared && !IsIconic(window)) {
   while (::task_get_run())
   {
      nextFrame(callback);
      //void * pData = nullptr;
      //vkMapMemory(m_pvulkandevice->logicalDevice, offscreenPass.color.mem, 0, destWidth * destHeight * 4, VK_MEMORY_MAP_PLACED_BIT_EXT, &pData);
      //callback(pData, destWidth, destHeight, destWidth * 4);
      //vkUnmapMemory(m_pvulkandevice->logicalDevice, offscreenPass.color.mem);
      preempt(20_ms);
   }
//    // SRS - for non-apple plaforms, handle benchmarking here within base_application_with_swap_chain::renderLoop()
//    //     - for macOS, handle benchmarking within NSApp rendering loop via displayLinkOutputCb()
// #if !(defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
//    if (benchmark.active) {
// #if defined(VK_USE_PLATFORM_WAYLAND_KHR)
//       while (!configured)
//          wl_display_dispatch(display);
//       while (wl_display_prepare_read(display) != 0)
//          wl_display_dispatch_pending(display);
//       wl_display_flush(display);
//       wl_display_read_events(display);
//       wl_display_dispatch_pending(display);
// #endif
//
//       benchmark.run([=] { render(callback); }, m_pvulkandevice->properties);
//       vkDeviceWaitIdle(device);
//       if (benchmark.filename != "") {
//          benchmark.saveResults();
//       }
//       return;
//    }
// #endif
//
//    destWidth = width;
//    destHeight = height;
//    lastTimestamp = std::chrono::high_resolution_clock::now();
//    tPrevEnd = lastTimestamp;
// #if defined(_WIN32)
//    //MSG msg;
//    //bool quitMessageReceived = false;
//    //while (!quitMessageReceived) {
//    //   while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
//    //      TranslateMessage(&msg);
//    //      DispatchMessage(&msg);
//    //      if (msg.message == WM_QUIT) {
//    //         quitMessageReceived = true;
//    //         break;
//    //      }
//    //   }
//    //   if (prepared && !IsIconic(window)) {
//    while (::task_get_run())
//    {
//       nextFrame(callback);
//       //void * pData = nullptr;
//       //vkMapMemory(m_pvulkandevice->logicalDevice, offscreenPass.color.mem, 0, destWidth * destHeight * 4, VK_MEMORY_MAP_PLACED_BIT_EXT, &pData);
//       //callback(pData, destWidth, destHeight, destWidth * 4);
//       //vkUnmapMemory(m_pvulkandevice->logicalDevice, offscreenPass.color.mem);
//       preempt(20_ms);
//    }
// #elif defined(VK_USE_PLATFORM_ANDROID_KHR)
//    while (1)
//    {
//       int ident;
//       int happenings;
//       struct android_poll_source * source;
//       bool destroy = false;
//
//       focused = true;
//
//       while ((ident = ALooper_pollAll(focused ? 0 : -1, NULL, &happenings, (void **)&source)) >= 0)
//       {
//          if (source != NULL)
//          {
//             source->process(androidApp, source);
//          }
//          if (androidApp->destroyRequested != 0)
//          {
//             LOGD("Android app destroy requested"_ansi);
//             destroy = true;
//             break;
//          }
//       }
//
//       // App destruction requested
//       // Exit loop, example will be destroyed in application main
//       if (destroy)
//       {
//          ANativeActivity_finish(androidApp->activity);
//          break;
//       }
//
//       // Render frame
//       if (prepared)
//       {
//          auto tStart = std::chrono::high_resolution_clock::now();
//          render();
//          frameCounter++;
//          auto tEnd = std::chrono::high_resolution_clock::now();
//          auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//          frameTimer = tDiff / 1000.0f;
//          camera.update(frameTimer);
//          // Convert to clamped timer value
//          if (!paused)
//          {
//             timer += timerSpeed * frameTimer;
//             if (timer > 1.0)
//             {
//                timer -= 1.0f;
//             }
//          }
//          float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
//          if (fpsTimer > 1000.0f)
//          {
//             lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
//             frameCounter = 0;
//             lastTimestamp = tEnd;
//          }
//
//          updateOverlay();
//
//          bool updateView = false;
//
//          // Check touch state (for movement)
//          if (touchDown) {
//             touchTimer += frameTimer;
//          }
//          if (touchTimer >= 1.0) {
//             camera.keys.up = true;
//          }
//
//          // Check gamepad state
//          const float deadZone = 0.0015f;
//          if (camera.type != Camera::CameraType::firstperson)
//          {
//             // Rotate
//             if (std::abs(gamePadState.axisLeft.x) > deadZone)
//             {
//                camera.rotate(glm::vec3(0.0f, gamePadState.axisLeft.x * 0.5f, 0.0f));
//                updateView = true;
//             }
//             if (std::abs(gamePadState.axisLeft.y) > deadZone)
//             {
//                camera.rotate(glm::vec3(gamePadState.axisLeft.y * 0.5f, 0.0f, 0.0f));
//                updateView = true;
//             }
//             // Zoom
//             if (std::abs(gamePadState.axisRight.y) > deadZone)
//             {
//                camera.translate(glm::vec3(0.0f, 0.0f, gamePadState.axisRight.y * 0.01f));
//                updateView = true;
//             }
//          }
//          else
//          {
//             updateView = camera.updatePad(gamePadState.axisLeft, gamePadState.axisRight, frameTimer);
//          }
//       }
//    }
// #elif defined(_DIRECT2DISPLAY)
//    while (!quit)
//    {
//       auto tStart = std::chrono::high_resolution_clock::now();
//       if (viewUpdated)
//       {
//          viewUpdated = false;
//       }
//       render();
//       frameCounter++;
//       auto tEnd = std::chrono::high_resolution_clock::now();
//       auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//       frameTimer = tDiff / 1000.0f;
//       camera.update(frameTimer);
//       if (camera.moving())
//       {
//          viewUpdated = true;
//       }
//       // Convert to clamped timer value
//       if (!paused)
//       {
//          timer += timerSpeed * frameTimer;
//          if (timer > 1.0)
//          {
//             timer -= 1.0f;
//          }
//       }
//       float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
//       if (fpsTimer > 1000.0f)
//       {
//          lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
//          frameCounter = 0;
//          lastTimestamp = tEnd;
//       }
//       updateOverlay();
//    }
// #elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
//    while (!quit)
//    {
//       auto tStart = std::chrono::high_resolution_clock::now();
//       if (viewUpdated)
//       {
//          viewUpdated = false;
//       }
//       DFBWindowEvent happening;
//       while (!event_buffer->GetEvent(event_buffer, DFB_EVENT(&happening)))
//       {
//          handleEvent(&happening);
//       }
//       render();
//       frameCounter++;
//       auto tEnd = std::chrono::high_resolution_clock::now();
//       auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//       frameTimer = tDiff / 1000.0f;
//       camera.update(frameTimer);
//       if (camera.moving())
//       {
//          viewUpdated = true;
//       }
//       // Convert to clamped timer value
//       if (!paused)
//       {
//          timer += timerSpeed * frameTimer;
//          if (timer > 1.0)
//          {
//             timer -= 1.0f;
//          }
//       }
//       float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
//       if (fpsTimer > 1000.0f)
//       {
//          lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
//          frameCounter = 0;
//          lastTimestamp = tEnd;
//       }
//       updateOverlay();
//    }
// #elif defined(VK_USE_PLATFORM_WAYLAND_KHR)
//    while (!quit)
//    {
//       auto tStart = std::chrono::high_resolution_clock::now();
//       if (viewUpdated)
//       {
//          viewUpdated = false;
//       }
//
//       while (!configured)
//          wl_display_dispatch(display);
//       while (wl_display_prepare_read(display) != 0)
//          wl_display_dispatch_pending(display);
//       wl_display_flush(display);
//       wl_display_read_events(display);
//       wl_display_dispatch_pending(display);
//
//       render();
//       frameCounter++;
//       auto tEnd = std::chrono::high_resolution_clock::now();
//       auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//       frameTimer = tDiff / 1000.0f;
//       camera.update(frameTimer);
//       if (camera.moving())
//       {
//          viewUpdated = true;
//       }
//       // Convert to clamped timer value
//       if (!paused)
//       {
//          timer += timerSpeed * frameTimer;
//          if (timer > 1.0)
//          {
//             timer -= 1.0f;
//          }
//       }
//       float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
//       if (fpsTimer > 1000.0f)
//       {
//          if (!settings.overlay)
//          {
//             std::string windowTitle = getWindowTitle();
//             xdg_toplevel_set_title(xdg_toplevel, windowTitle.c_str());
//          }
//          lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
//          frameCounter = 0;
//          lastTimestamp = tEnd;
//       }
//       updateOverlay();
//    }
// #elif defined(VK_USE_PLATFORM_XCB_KHR)
//    xcb_flush(connection);
//    while (!quit)
//    {
//       auto tStart = std::chrono::high_resolution_clock::now();
//       if (viewUpdated)
//       {
//          viewUpdated = false;
//       }
//       xcb_generic_event_t * happening;
//       while ((happening = xcb_poll_for_event(connection)))
//       {
//          handleEvent(happening);
//          free(happening);
//       }
//       render();
//       frameCounter++;
//       auto tEnd = std::chrono::high_resolution_clock::now();
//       auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//       frameTimer = tDiff / 1000.0f;
//       camera.update(frameTimer);
//       if (camera.moving())
//       {
//          viewUpdated = true;
//       }
//       // Convert to clamped timer value
//       if (!paused)
//       {
//          timer += timerSpeed * frameTimer;
//          if (timer > 1.0)
//          {
//             timer -= 1.0f;
//          }
//       }
//       float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
//       if (fpsTimer > 1000.0f)
//       {
//          if (!settings.overlay)
//          {
//             std::string windowTitle = getWindowTitle();
//             xcb_change_property(connection, XCB_PROP_MODE_REPLACE,
//                window, XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8,
//                windowTitle.size(), windowTitle.c_str());
//          }
//          lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
//          frameCounter = 0;
//          lastTimestamp = tEnd;
//       }
//       updateOverlay();
//    }
// #elif defined(VK_USE_PLATFORM_HEADLESS_EXT)
//    while (!quit)
//    {
//       auto tStart = std::chrono::high_resolution_clock::now();
//       if (viewUpdated)
//       {
//          viewUpdated = false;
//       }
//       render();
//       frameCounter++;
//       auto tEnd = std::chrono::high_resolution_clock::now();
//       auto tDiff = std::chrono::duration<double, std::milli>(tEnd - tStart).count();
//       frameTimer = tDiff / 1000.0f;
//       camera.update(frameTimer);
//       if (camera.moving())
//       {
//          viewUpdated = true;
//       }
//       // Convert to clamped timer value
//       timer += timerSpeed * frameTimer;
//       if (timer > 1.0)
//       {
//          timer -= 1.0f;
//       }
//       float fpsTimer = std::chrono::duration<double, std::milli>(tEnd - lastTimestamp).count();
//       if (fpsTimer > 1000.0f)
//       {
//          lastFPS = (float)frameCounter * (1000.0f / fpsTimer);
//          frameCounter = 0;
//          lastTimestamp = tEnd;
//       }
//       updateOverlay();
//    }
// #elif (defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT)) && defined(VK_EXAMPLE_XCODE_GENERATED)
//    [NSApp run];
// #elif defined(VK_USE_PLATFORM_SCREEN_QNX)
//    while (!quit) {
//       handleEvent();
//
//       if (prepared) {
//          nextFrame();
//       }
//    }
// #endif
   // Flush device to make sure all resources can be freed
   if (device != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(device);
   }
}


//offscreen_application * VulkanExample;
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//   if (VulkanExample != NULL)
//   {
//      offscreen_application->handleMessages(hWnd, uMsg, wParam, lParam);
//   }
//   return (DefWindowProc(hWnd, uMsg, wParam, lParam));
//}


::pointer<::vulkan::application > start_offscreen_application(::vkc::VkContainer * pvkcontainer, mouseState * pmousestate)
{

   auto pvulkanoffscreenapplication = pvkcontainer->__create_new < offscreen_application >();

   pvulkanoffscreenapplication->m_pmousestate = pmousestate;

   pvulkanoffscreenapplication->initVulkan();

   // VulkanExample->setupWindow(::GetModuleHandleW(L"app_core_vulken.dll"_ansi), WndProc);

   pvulkanoffscreenapplication->prepare();

//   pvulkanexample->renderLoop20(callback);
   //delete(VulkanExample);
   
   return pvulkanoffscreenapplication;

}


} // namespace vulkan


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
//offscreen_application *offscreen_application;																		\
//LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)						\
//{																									\
//	if (offscreen_application != NULL)																		\
//	{																								\
//		offscreen_application->handleMessages(hWnd, uMsg, wParam, lParam);									\
//	}																								\
//	return (DefWindowProc(hWnd, uMsg, wParam, lParam));												\
//}																									\
//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)									\
//{																									\
//	for (int32_t i = 0; i < __argc; i++) { offscreen_application::args.push_back(__argv[i]); };  			\
//	offscreen_application = ___new offscreen_application();															\
//	offscreen_application->initVulkan();																	\
//	offscreen_application->setupWindow(hInstance, WndProc);													\
//	offscreen_application->prepare();																		\
//	offscreen_application->renderLoop();																	\
//	delete(offscreen_application);																			\
//	return 0;																						\
//}
//
//#elif defined(VK_USE_PLATFORM_ANDROID_KHR)
// /*
//  * Android
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//offscreen_application *offscreen_application;																		\
//void android_main(android_app* state)																\
//{																									\
//	offscreen_application = ___new offscreen_application();															\
//	state->userData = offscreen_application;																\
//	state->onAppCmd = offscreen_application::handleAppCommand;												\
//	state->onInputEvent = offscreen_application::handleAppInput;											\
//	androidApp = state;																				\
//	vks::android::getDeviceConfig();																\
//	offscreen_application->renderLoop();																	\
//	delete(offscreen_application);																			\
//}

//#elif defined(_DIRECT2DISPLAY)
// /*
//  * Direct-to-display
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//offscreen_application *offscreen_application;																		\
//static void handleEvent()                                											\
//{																									\
//}																									\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { offscreen_application::args.push_back(argv[i]); };  				\
//	offscreen_application = ___new offscreen_application();															\
//	offscreen_application->initVulkan();																	\
//	offscreen_application->prepare();																		\
//	offscreen_application->renderLoop();																	\
//	delete(offscreen_application);																			\
//	return 0;																						\
//}

//#elif defined(VK_USE_PLATFORM_DIRECTFB_EXT)
// /*
//  * Direct FB
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//offscreen_application *offscreen_application;																		\
//static void handleEvent(const DFBWindowEvent *happening)												\
//{																									\
//	if (offscreen_application != NULL)																		\
//	{																								\
//		offscreen_application->handleEvent(happening);															\
//	}																								\
//}																									\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { offscreen_application::args.push_back(argv[i]); };  				\
//	offscreen_application = ___new offscreen_application();															\
//	offscreen_application->initVulkan();																	\
//	offscreen_application->setupWindow();					 												\
//	offscreen_application->prepare();																		\
//	offscreen_application->renderLoop();																	\
//	delete(offscreen_application);																			\
//	return 0;																						\
//}

//#elif (defined(VK_USE_PLATFORM_WAYLAND_KHR) || defined(VK_USE_PLATFORM_HEADLESS_EXT))
// /*
//  * Wayland / headless
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//offscreen_application *offscreen_application;																		\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { offscreen_application::args.push_back(argv[i]); };  				\
//	offscreen_application = ___new offscreen_application();															\
//	offscreen_application->initVulkan();																	\
//	offscreen_application->setupWindow();					 												\
//	offscreen_application->prepare();																		\
//	offscreen_application->renderLoop();																	\
//	delete(offscreen_application);																			\
//	return 0;																						\
//}

//#elif defined(VK_USE_PLATFORM_XCB_KHR)
// /*
//  * X11 Xcb
//  */
//#define VULKAN_EXAMPLE_MAIN()																		\
//offscreen_application *offscreen_application;																		\
//static void handleEvent(const xcb_generic_event_t *happening)											\
//{																									\
//	if (offscreen_application != NULL)																		\
//	{																								\
//		offscreen_application->handleEvent(happening);															\
//	}																								\
//}																									\
//int main(const int argc, const char *argv[])													    \
//{																									\
//	for (size_t i = 0; i < argc; i++) { offscreen_application::args.push_back(argv[i]); };  				\
//	offscreen_application = ___new offscreen_application();															\
//	offscreen_application->initVulkan();																	\
//	offscreen_application->setupWindow();					 												\
//	offscreen_application->prepare();																		\
//	offscreen_application->renderLoop();																	\
//	delete(offscreen_application);																			\
//	return 0;																						\
//}

//#elif (defined(VK_USE_PLATFORM_IOS_MVK) || defined(VK_USE_PLATFORM_MACOS_MVK) || defined(VK_USE_PLATFORM_METAL_EXT))
// /*
//  * iOS and macOS (using MoltenVK)
//  */
//#if defined(VK_EXAMPLE_XCODE_GENERATED)
//#define VULKAN_EXAMPLE_MAIN()																		\
//offscreen_application *offscreen_application;																		\
//int main(const int argc, const char *argv[])														\
//{																									\
//	@autoreleasepool																				\
//	{																								\
//		for (size_t i = 0; i < argc; i++) { offscreen_application::args.push_back(argv[i]); };				\
//		offscreen_application = ___new offscreen_application();														\
//		offscreen_application->initVulkan();																\
//		offscreen_application->setupWindow(nullptr);														\
//		offscreen_application->prepare();																	\
//		offscreen_application->renderLoop();																\
//		delete(offscreen_application);																		\
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
//offscreen_application *offscreen_application;																		
//int main(const int argc, const char *argv[])														
//{																									
//	for (int i = 0; i < argc; i++) { offscreen_application::args.push_back(argv[i]); };						
//	offscreen_application = ___new offscreen_application();															
//	offscreen_application->initVulkan();																	
//	offscreen_application->setupWindow();																	
//	offscreen_application->prepare();																		
//	offscreen_application->renderLoop();																	
//	delete(offscreen_application);																			
//	return 0;																						
//}
//
//
//

