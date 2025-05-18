#include "framework.h"
#include "renderer.h"
#include "offscreen_render_pass.h"
#include "swap_chain_render_pass.h"
#include "initializers.h"
#include "tools.h"
#include "app-cube/cube/impact.h"

#include <array>
#include <cassert>
#include <stdexcept>



namespace graphics3d_vulkan
{

   // renderer::renderer(VkWindow& window, context* pvkcdevice) : vkcWindow{ window }, m_pcontext{ pvkcdevice } 
   renderer::renderer()
   {

   }


   void renderer::initialize_renderer(::cube::impact* pimpact, ::cube::context* pcontext)
   {

      m_pimpact = pimpact;

      m_pcontext = pcontext;

      __construct_new(m_poffscreensampler);

      m_poffscreensampler->initialize_offscreen_sampler(pcontext);

      defer_layout();

      createCommandBuffers();

   }


   renderer::~renderer() {
      freeCommandBuffers();
   }


   void renderer::defer_layout()
   {

      auto size = m_pimpact->size();

      if (m_extentRenderer.width == size.width()
         && m_extentRenderer.height == size.height())
      {

         return;

      }

      m_extentRenderer.width = size.width();
      m_extentRenderer.height = size.height();

      if (m_bOffScreen)
      {

         m_pvkcrenderpass = __allocate offscreen_render_pass(m_pcontext, m_extentRenderer, m_pvkcrenderpass);

      }

      m_pvkcrenderpass->init();

      //auto extent = vkcWindow.getExtent();
      //while (extent.width == 0 || extent.height == 0) {
      //	glfwWaitEvents();
      //}
      //vkDeviceWaitIdle(m_pcontext->logicalDevice());

      //if (vkcSwapChain == nullptr) {
      //	vkcSwapChain = std::make_unique<swap_chain_render_pass>(m_pcontext, extent);
      //}
      //else {
      //	::pointer<swap_chain_render_pass> oldSwapChain = std::move(vkcSwapChain);
      //	vkcSwapChain = std::make_unique<swap_chain_render_pass>(m_pcontext, extent, oldSwapChain);
      //	if (!oldSwapChain->compareSwapFormats(*vkcSwapChain.get())) {
      //		throw std::runtime_error("Swap chain image(or depth) format has changed!");
      //	}

      //}
   }

   void renderer::createCommandBuffers() {
      commandBuffers.resize(render_pass::MAX_FRAMES_IN_FLIGHT);

      VkCommandBufferAllocateInfo allocInfo{};
      allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandPool = m_pcontext->getCommandPool();
      allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

      if (vkAllocateCommandBuffers(m_pcontext->logicalDevice(), &allocInfo, commandBuffers.data()) !=
         VK_SUCCESS) {
         throw std::runtime_error("failed to allocate command buffers!");
      }

   }

   void renderer::freeCommandBuffers() {
      vkFreeCommandBuffers(
         m_pcontext->logicalDevice(),
         m_pcontext->getCommandPool(),
         static_cast<float>(commandBuffers.size()),
         commandBuffers.data());
      commandBuffers.clear();
   }

   VkCommandBuffer renderer::beginFrame()
   {

      defer_layout();
      
      assert(!isFrameStarted && "Can't call beginFrame while already in progress");

      //if (m_bOffScreen)
      {

         auto result = m_pvkcrenderpass->acquireNextImage(&currentImageIndex);

         if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            defer_layout();
            return nullptr;
         }
         if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("Failed to aquire swap chain image");
         }

         isFrameStarted = true;

         auto commandBuffer = getCurrentCommandBuffer();

         VkCommandBufferBeginInfo beginInfo{};
         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

         if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
         }
         return commandBuffer;

      }
      //else
      //{


      //	auto result = m_pvkcswapchain->acquireNextImage(&currentImageIndex);

      //	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
      //		recreateRenderPass();
      //		return nullptr;
      //	}
      //	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
      //		throw std::runtime_error("Failed to aquire swap chain image");
      //	}

      //	isFrameStarted = true;

      //	auto commandBuffer = getCurrentCommandBuffer();

      //	VkCommandBufferBeginInfo beginInfo{};
      //	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

      //	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
      //		throw std::runtime_error("failed to begin recording command buffer!");
      //	}
      //	return commandBuffer;

      //}
   }


   renderer::OffScreenSampler::OffScreenSampler()
   {

      clear();

   }


   renderer::OffScreenSampler::~OffScreenSampler()
   {

      destroy();

   }


   void renderer::OffScreenSampler::initialize_offscreen_sampler(::cube::context* pcontext)
   {

      m_pcontext = pcontext;

   }


   void renderer::OffScreenSampler::clear()
   {

      m_vkextent2d.width = 0;
      m_vkextent2d.height = 0;
      m_vkdevicememory = nullptr;
      m_vkimage = nullptr;

   }


   void renderer::OffScreenSampler::update(VkExtent2D vkextent2d)
   {

      if (vkextent2d.width == m_vkextent2d.width
         && vkextent2d.height == m_vkextent2d.height)
      {

         return;

      }

      destroy();

      if (vkextent2d.width <= 0 ||
         vkextent2d.height <= 0)
      {

         return;

      }

      m_vkextent2d.width = vkextent2d.width;
      m_vkextent2d.height = vkextent2d.height;

      VkImageCreateInfo imgCreateInfo(initializers::image_create_info());
      imgCreateInfo.imageType = VK_IMAGE_TYPE_2D;
      imgCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
      imgCreateInfo.extent.width = m_vkextent2d.width;
      imgCreateInfo.extent.height = m_vkextent2d.height;
      imgCreateInfo.extent.depth = 1;
      imgCreateInfo.arrayLayers = 1;
      imgCreateInfo.mipLevels = 1;
      imgCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imgCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
      imgCreateInfo.tiling = VK_IMAGE_TILING_LINEAR;
      imgCreateInfo.usage = VK_IMAGE_USAGE_TRANSFER_DST_BIT;
      // Create the image
      VkImage dstImage;
      VK_CHECK_RESULT(vkCreateImage(m_pcontext->logicalDevice(), &imgCreateInfo, nullptr, &m_vkimage));
      // Create memory to back up the image
      VkMemoryRequirements memRequirements;
      VkMemoryAllocateInfo memAllocInfo(initializers::memory_allocate_info());
      vkGetImageMemoryRequirements(m_pcontext->logicalDevice(), m_vkimage, &memRequirements);
      memAllocInfo.allocationSize = memRequirements.size;
      // Memory must be host visible to copy from
      memAllocInfo.memoryTypeIndex = m_pcontext->findMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
      VK_CHECK_RESULT(vkAllocateMemory(m_pcontext->logicalDevice(), &memAllocInfo, nullptr, &m_vkdevicememory));
      VK_CHECK_RESULT(vkBindImageMemory(m_pcontext->logicalDevice(), m_vkimage, m_vkdevicememory, 0));


   }


   void renderer::OffScreenSampler::destroy()
   {

      if (m_vkdevicememory)
      {

         vkFreeMemory(m_pcontext->logicalDevice(), m_vkdevicememory, nullptr);
         vkDestroyImage(m_pcontext->logicalDevice(), m_vkimage, nullptr);

         clear();

      }

   }


   void renderer::OffScreenSampler::sample(VkImage vkimage)
   {

      if (!m_vkimage || !m_vkdevicememory)
      {

         return;

      }


      // Create the linear tiled destination image to copy to and to read the memory from

// Do the actual blit from the offscreen image to our host visible destination image
      VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::command_buffer_allocate_info(m_pcontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
      VkCommandBuffer copyCmd;
      VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pcontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
      VkCommandBufferBeginInfo cmdBufInfo = initializers::command_buffer_begin_info();
      VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

      // Transition destination image to transfer destination layout
      ::graphics3d_vulkan::tools::insertImageMemoryBarrier(
         copyCmd,
         m_vkimage,
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
      imageCopyRegion.extent.width = m_vkextent2d.width;
      imageCopyRegion.extent.height = m_vkextent2d.height;
      imageCopyRegion.extent.depth = 1;

      vkCmdCopyImage(
         copyCmd,
         //colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         vkimage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
         m_vkimage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         1,
         &imageCopyRegion);

      // Transition destination image to general layout, which is the required layout for mapping the image memory later on
      ::graphics3d_vulkan::tools::insertImageMemoryBarrier(
         copyCmd,
         m_vkimage,
         VK_ACCESS_TRANSFER_WRITE_BIT,
         VK_ACCESS_MEMORY_READ_BIT,
         VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
         VK_IMAGE_LAYOUT_GENERAL,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VK_PIPELINE_STAGE_TRANSFER_BIT,
         VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

      VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

      m_pcontext->submitWork(copyCmd, m_pcontext->graphicsQueue());

   }


   void renderer::OffScreenSampler::send_sample()
   {

      if (!m_vkimage || !m_vkdevicememory)
      {

         return;

      }

      auto pimpact = m_pcontext->m_pimpact;

      auto callback = pimpact->m_callbackOffscreen;

      // Get layout of the image (including row pitch)
      VkImageSubresource subResource{};
      subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      VkSubresourceLayout subResourceLayout;

      vkGetImageSubresourceLayout(m_pcontext->logicalDevice(), m_vkimage, &subResource, &subResourceLayout);

      const char* imagedata = nullptr;
      // Map image memory so we can start copying from it
      vkMapMemory(m_pcontext->logicalDevice(), m_vkdevicememory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
      imagedata += subResourceLayout.offset;

      /*
         Save host visible framebuffer image to disk (ppm format)
      */

      //::memory mem;

      //mem.set_size(m_width * m_height * 4);

      //std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
      //const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
      callback((void*)imagedata,
         m_vkextent2d.width,
         m_vkextent2d.height,
         subResourceLayout.rowPitch);


      vkUnmapMemory(m_pcontext->logicalDevice(), m_vkdevicememory);

   }


   void renderer::sample()
   {

      auto callback = m_pimpact->m_callbackOffscreen;

      if (callback)
      {

         m_poffscreensampler->update(m_pvkcrenderpass->getExtent());
         /*
            Copy framebuffer image to host visible image
         */
         /*const char* imagedata;*/
         {

            m_poffscreensampler->sample(m_pvkcrenderpass->m_images[currentImageIndex]);

            //// Create the linear tiled destination image to copy to and to read the memory from

            //// Do the actual blit from the offscreen image to our host visible destination image
            //VkCommandBufferAllocateInfo cmdBufAllocateInfo = initializers::commandBufferAllocateInfo(m_pcontext->getCommandPool(), VK_COMMAND_BUFFER_LEVEL_PRIMARY, 1);
            //VkCommandBuffer copyCmd;
            //VK_CHECK_RESULT(vkAllocateCommandBuffers(m_pcontext->logicalDevice(), &cmdBufAllocateInfo, &copyCmd));
            //VkCommandBufferBeginInfo cmdBufInfo = initializers::commandBufferBeginInfo();
            //VK_CHECK_RESULT(vkBeginCommandBuffer(copyCmd, &cmdBufInfo));

            //// Transition destination image to transfer destination layout
            //::graphics3d_vulkan::tools::insertImageMemoryBarrier(
            //	copyCmd,
            //	m_poffscreensampler->m_vkimage,
            //	0,
            //	VK_ACCESS_TRANSFER_WRITE_BIT,
            //	VK_IMAGE_LAYOUT_UNDEFINED,
            //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            //// colorAttachment.image is already in VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, and does not need to be transitioned

            //VkImageCopy imageCopyRegion{};
            //imageCopyRegion.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            //imageCopyRegion.srcSubresource.layerCount = 1;
            //imageCopyRegion.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            //imageCopyRegion.dstSubresource.layerCount = 1;
            //imageCopyRegion.extent.width = m_pvkcrenderpass->width();
            //imageCopyRegion.extent.height = m_pvkcrenderpass->height();
            //imageCopyRegion.extent.depth = 1;

            //vkCmdCopyImage(
            //	copyCmd,
            //	//colorAttachment.image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            //	m_pvkcrenderpass->m_images[iIndex], VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
            //	dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //	1,
            //	&imageCopyRegion);

            //// Transition destination image to general layout, which is the required layout for mapping the image memory later on
            //::graphics3d_vulkan::tools::insertImageMemoryBarrier(
            //	copyCmd,
            //	dstImage,
            //	VK_ACCESS_TRANSFER_WRITE_BIT,
            //	VK_ACCESS_MEMORY_READ_BIT,
            //	VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            //	VK_IMAGE_LAYOUT_GENERAL,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VK_PIPELINE_STAGE_TRANSFER_BIT,
            //	VkImageSubresourceRange{ VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 });

            //VK_CHECK_RESULT(vkEndCommandBuffer(copyCmd));

            //submitWork(copyCmd, queue);

            //submitInfo.commandBufferCount = 1;
            //submitInfo.pCommandBuffers = &copyCmd;
            //VK_CHECK_RESULT(vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE));


            //// Get layout of the image (including row pitch)
            //VkImageSubresource subResource{};
            //subResource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            //VkSubresourceLayout subResourceLayout;

            //vkGetImageSubresourceLayout(m_pcontext->logicalDevice(), dstImage, &subResource, &subResourceLayout);

            //// Map image memory so we can start copying from it
            //vkMapMemory(m_pcontext->logicalDevice(), dstImageMemory, 0, VK_WHOLE_SIZE, 0, (void**)&imagedata);
            //imagedata += subResourceLayout.offset;

            m_poffscreensampler->send_sample();

            ///*
            //	Save host visible framebuffer image to disk (ppm format)
            //*/

            ////::memory mem;

            ////mem.set_size(m_width * m_height * 4);

            ////std::vector<VkFormat> formatsBGR = { VK_FORMAT_B8G8R8A8_SRGB, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_B8G8R8A8_SNORM };
            ////const bool colorSwizzle = (std::find(formatsBGR.begin(), formatsBGR.end(), VK_FORMAT_R8G8B8A8_UNORM) != formatsBGR.end());
            //if (callback)
            //{
            //	callback((void*)imagedata, 
            //		m_pvkcrenderpass->width(),
            //		m_pvkcrenderpass->height(),
            //		subResourceLayout.rowPitch);

            //}

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
         }


      }
       vkQueueWaitIdle(m_pcontext->graphicsQueue());


   }


   void renderer::endFrame()
   {

      //if (m_bOffScreen)
      {

         assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
         auto commandBuffer = getCurrentCommandBuffer();
         if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
         }
         auto result = m_pvkcrenderpass->submitCommandBuffers(&commandBuffer, &currentImageIndex);
         //if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
         //	vkcWindow.wasWindowResized()) 
         //{
         //	vkcWindow.resetWindowResizedFlag();
         //	recreateSwapchain();
         //}
         //else 
         //	if (result != VK_SUCCESS) {
         //	throw std::runtime_error("failed to present swap chain image!");
         //}
         sample();
         isFrameStarted = false;
         currentFrameIndex = (currentFrameIndex + 1) % render_pass::MAX_FRAMES_IN_FLIGHT;

      }
      //else
      //{


      //	assert(isFrameStarted && "Can't call endFrame while frame is not in progress");
      //	auto commandBuffer = getCurrentCommandBuffer();
      //	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
      //		throw std::runtime_error("failed to record command buffer!");
      //	}
      //	auto result = m_pvkcswapchain->submitCommandBuffers(&commandBuffer, &currentImageIndex);
      //	//if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ||
      //	//	vkcWindow.wasWindowResized()) 
      //	//{
      //	//	vkcWindow.resetWindowResizedFlag();
      //	//	recreateSwapchain();
      //	//}
      //	//else 
      //	//	if (result != VK_SUCCESS) {
      //	//	throw std::runtime_error("failed to present swap chain image!");
      //	//}
      //	isFrameStarted = false;
      //	currentFrameIndex = (currentFrameIndex + 1) % swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT;

      //}

   }


   void renderer::beginRenderPass(VkCommandBuffer commandBuffer)
   {

      //if (m_bOffScreen)
      {

         assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
         assert(
            commandBuffer == getCurrentCommandBuffer() &&
            "Can't begin render pass on command buffer from a different frame");

         VkRenderPassBeginInfo renderPassInfo{};
         renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
         renderPassInfo.renderPass = m_pvkcrenderpass->getRenderPass();
         renderPassInfo.framebuffer = m_pvkcrenderpass->getFrameBuffer(currentImageIndex);

         renderPassInfo.renderArea.offset = { 0, 0 };
         renderPassInfo.renderArea.extent = m_pvkcrenderpass->getExtent();

         std::array<VkClearValue, 2> clearValues{};
         //clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
         clearValues[0].color = { 0.5f, 0.75f, 1.0f, 1.0f };
         clearValues[1].depthStencil = { 1.0f, 0 };
         renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
         renderPassInfo.pClearValues = clearValues.data();

         vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

         VkViewport viewport{};
         viewport.x = 0.0f;
         viewport.y = 0.0f;
         viewport.width = static_cast<float>(m_pvkcrenderpass->getExtent().width);
         viewport.height = static_cast<float>(m_pvkcrenderpass->getExtent().height);
         viewport.minDepth = 0.0f;
         viewport.maxDepth = 1.0f;
         VkRect2D scissor{ {0, 0}, m_pvkcrenderpass->getExtent() };
         vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
         vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

      }
      //else
      //{

      //	assert(isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
      //	assert(
      //		commandBuffer == getCurrentCommandBuffer() &&
      //		"Can't begin render pass on command buffer from a different frame");

      //	VkRenderPassBeginInfo renderPassInfo{};
      //	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
      //	renderPassInfo.renderPass = m_pvkcswapchain->getRenderPass();
      //	renderPassInfo.framebuffer = m_pvkcswapchain->getFrameBuffer(currentImageIndex);

      //	renderPassInfo.renderArea.offset = { 0, 0 };
      //	renderPassInfo.renderArea.extent = m_pvkcswapchain->getExtent();

      //	std::array<VkClearValue, 2> clearValues{};
      //	clearValues[0].color = { 2.01f, 0.01f, 0.01f, 1.0f };
      //	clearValues[1].depthStencil = { 1.0f, 0 };
      //	renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
      //	renderPassInfo.pClearValues = clearValues.data();

      //	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

      //	VkViewport viewport{};
      //	viewport.x = 0.0f;
      //	viewport.y = 0.0f;
      //	viewport.width = static_cast<float>(vkcSwapChain->getSwapChainExtent().width);
      //	viewport.height = static_cast<float>(vkcSwapChain->getSwapChainExtent().height);
      //	viewport.minDepth = 0.0f;
      //	viewport.maxDepth = 1.0f;
      //	VkRect2D scissor{ {0, 0}, vkcSwapChain->getSwapChainExtent() };
      //	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
      //	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);


      //}
   }


   void renderer::endRenderPass(VkCommandBuffer commandBuffer)
   {

      assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
      assert(
         commandBuffer == getCurrentCommandBuffer() &&
         "Can't end render pass on command buffer from a different frame");
      vkCmdEndRenderPass(commandBuffer);
   }


} // namespace graphics3d_vulkan



