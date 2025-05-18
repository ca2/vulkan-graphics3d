// From application_object by camilo on 2025-05-17 01:10 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "descriptors.h"
#include "buffer.h"
#include "engine.h"
#include "frame_info.h"
#include "input.h"
#include "offscreen_render_pass.h"
#include "renderer.h"
#include "swap_chain_render_pass.h"
#include "app-cube/cube/application.h"
#include "app-cube/cube/impact.h"
#include "app-cube/cube/graphics3d/camera.h"
#include "app-cube/cube/graphics3d/scene.h"
#include "system/basic_render_system.h"
#include "system/point_light_system.h"
#include "acme/platform/application.h"
#include "apex/database/client.h"
#include "apex/database/stream.h"
//#include "app-cube/cube/container.h"
#include <chrono>


namespace graphics3d_vulkan
{



   engine::engine()
   {


   }


   engine::~engine()
   {


   }


   void engine::run()
   {

      auto papp = get_app();

      __øconstruct(m_pcontext);

      m_pcontext->initialize_context(papp->m_pimpact);

      __construct_new(m_prenderer);

      m_prenderer->initialize_renderer(papp->m_pimpact, m_pcontext);

      auto pglobalpoolbuilder = __allocate descriptor_pool::Builder();

      pglobalpoolbuilder->initialize_builder(m_pcontext);
      pglobalpoolbuilder->setMaxSets(render_pass::MAX_FRAMES_IN_FLIGHT);
      pglobalpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render_pass::MAX_FRAMES_IN_FLIGHT);

      m_pglobalpool = pglobalpoolbuilder->build();

      //m_pglobalpool->initialize_pool(pcontext);

      //= __allocate
      //   descriptor_pool::Builder(pcontext)
      //   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      //   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      //   .build();
      m_pscene->on_load_scene();

      //pcontext = __allocate context(m_pvulkandevice);

      ::pointer_array<buffer> uboBuffers;

      uboBuffers.set_size(render_pass::MAX_FRAMES_IN_FLIGHT);

      ::cast < context > pcontext = m_pcontext;

      for (int i = 0; i < uboBuffers.size(); i++)
      {

         uboBuffers[i] = __allocate buffer();

         uboBuffers[i]->initialize_buffer(
            pcontext,
            sizeof(GlobalUbo),
            1,
            VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

         uboBuffers[i]->map();

      }
      auto globalSetLayout = set_descriptor_layout::Builder(pcontext)
         .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
         .build();


      std::vector<VkDescriptorSet> globalDescriptorSets(render_pass::MAX_FRAMES_IN_FLIGHT);

      for (int i = 0; i < globalDescriptorSets.size(); i++)
      {

         auto bufferInfo = uboBuffers[i]->descriptorInfo();

         descriptor_writer(*globalSetLayout, *m_pglobalpool)
            .writeBuffer(0, &bufferInfo)
            .build(globalDescriptorSets[i]);

      }

      SimpleRenderSystem simpleRenderSystem{
          pcontext,
          m_prenderer->getRenderPass(),
          globalSetLayout->getDescriptorSetLayout() };

      point_light_system pointLightSystem{
          pcontext,
          m_prenderer->getRenderPass(),
          globalSetLayout->getDescriptorSetLayout()
      };

      //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      //{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      auto camera = m_pscene->get_default_camera();

      //VkcCamera camera(glm::vec3(0.0f, 2.0f, -10.0f), .0f, 0.0f);

      auto viewerObject = __øcreate <::graphics3d::scene_object>();
      papp->m_pimpact->m_bLastMouse = true;
      viewerObject->m_transform.translation.z = -2.5f;
      MNKController cameraController;

      cameraController.m_pimpact = papp->m_pimpact;
      cameraController.m_pkeymap = papp->m_pimpact->m_pkeymap;
      /*    glfwSetInputMode(_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
          glfwSetWindowUserPointer(_window.getGLFWwindow(), &cameraController);*/
      cameraController.m_bMouseAbsolute;

      ::pointer <::database::client> pdatabaseclient = m_papplication;

      if (pdatabaseclient)
      {

         pdatabaseclient->datastream()->get_block("camera", as_memory_block(camera));
         pdatabaseclient->datastream()->get_block("transform", as_memory_block(viewerObject->m_transform));
         pdatabaseclient->datastream()->get_block("camera_controller", cameraController.as_block());

      }

      auto pimpact = papp->m_pimpact;

      auto currentTime = std::chrono::high_resolution_clock::now();
      //while (!_window.shouldClose())
      while (!pimpact->m_bShouldClose && task_get_run())
      {

         task_iteration();
         //glfwPollEvents();

         auto newTime = std::chrono::high_resolution_clock::now();

         float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

         currentTime = newTime;

         cameraController.handleMouseInput();

         cameraController.updateLook(cameraController.getX(), cameraController.getY(), viewerObject);

         cameraController.updateMovement(frameTime, viewerObject);

         //cameraController.moveInPlaneXZ(m_pimpact, frameTime, viewerObject);

         camera.setViewYXZ(viewerObject->m_transform.translation, viewerObject->m_transform.rotation);

         if (m_prenderer->m_pvkcrenderpass->width() > 0
            && m_prenderer->m_pvkcrenderpass->height() > 0)
         {

            float aspect = m_prenderer->getAspectRatio();

            camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

            if (auto commandBuffer = m_prenderer->beginFrame())
            {

               int frameIndex = m_prenderer->getFrameIndex();

               FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], m_pscene->m_mapObjects };

               // update
               GlobalUbo ubo{};
               ubo.projection = camera.getProjection();
               ubo.view = camera.getView();
               ubo.inverseView = camera.getInverseView();
               pointLightSystem.update(frameInfo, ubo);
               uboBuffers[frameIndex]->writeToBuffer(&ubo);
               uboBuffers[frameIndex]->flush();

               // render
               m_prenderer->beginRenderPass(commandBuffer);

               simpleRenderSystem.renderGameObjects(frameInfo);
               pointLightSystem.render(frameInfo);

               m_prenderer->endRenderPass(commandBuffer);
               m_prenderer->endFrame();

            }

         }

      }

      if (pdatabaseclient)
      {

         pdatabaseclient->datastream()->set("camera_controller", cameraController.as_block());
         pdatabaseclient->datastream()->set("transform", as_memory_block(viewerObject->m_transform));
         pdatabaseclient->datastream()->set("camera", as_memory_block(camera));

      }

      if (pcontext->logicalDevice() != VK_NULL_HANDLE)
      {

         vkDeviceWaitIdle(pcontext->logicalDevice());

      }



   }


} // graphics3d_vulkan


