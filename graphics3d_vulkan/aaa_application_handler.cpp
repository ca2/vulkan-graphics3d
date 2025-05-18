#include "framework.h"
// vk_core.cpp
//#include "shared.h"
#include "application_handler.h"
#include "apex/database/stream.h"
// std
#include <iostream>
// std
#include <iostream>
#include <chrono>
#include <array>
#include <cassert>
#include <stdexcept>


// Vulkan
#include <vulkan/vulkan.h>


// Third party
//#define VMA_IMPLEMENTATION
//#include <vma/vk_mem_alloc.h>

#define GLM_FORCE_RADIANS	
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// Project source specific includes
#include "application_handler.h"
#include "vkbootstrap/VkBootstrap.h"
#include "buffer.h"
#include "app-cube/cube/camera.h"
#include "input.h"
//#include "renderer/Types/vk_basicRenderSystem.h"
//#include "renderer/Types/vk_pointLightSystem.h"



namespace graphics3d_vulkan 
{


   //::pointer<::vulkan::application > start_vulkan_application(::::cube::container* pcontainer, mouse_state* pmousestate)
   //{
   //   auto pvulkanexample = pcontainer->__create_new < application_handler >();

   //   pvulkanexample->initialize_application(pcontainer);

   //   pvulkanexample->m_pmousestate = pmousestate;
   //   //pvulkanexample->initVulkan();

   //   // VulkanExample->setupWindow(::GetModuleHandleW(L"app_core_vulken.dll"_ansi), WndProc);
   //   //pvulkanexample->prepare();
   //   //   pvulkanexample->renderLoop20(callback);
   //      //delete(VulkanExample);
   //   return pvulkanexample;
   //}


   application_handler::application_handler() 
   {


   }
   
   
   application_handler::~application_handler() 
   {


   }


   void application_handler::initialize_application(::cube::container* pcontainer)
   {

      m_p3dapplication->initialize_application(pcontainer);

   }


   void application_handler::run_application()
   {


      m_p3dapplication->run_application();

      //::pointer < vulkan_land_continuum::application> papp = m_p3dapplication;


      //__construct_new(m_pcontext);

      //m_pcontext->initialize_device(m_pcontainer);

      //__construct_new(m_prenderer);

      //m_prenderer->initialize_renderer(m_pcontainer, m_pcontext);

      //auto pglobalpoolbuilder = __allocate descriptor_pool::Builder();

      //pglobalpoolbuilder->initialize_builder(m_pcontext);
      //pglobalpoolbuilder->setMaxSets(render_pass::MAX_FRAMES_IN_FLIGHT);
      //pglobalpoolbuilder->addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, render_pass::MAX_FRAMES_IN_FLIGHT);

      //m_pglobalpool = pglobalpoolbuilder->build();

      ////m_pglobalpool->initialize_pool(m_pcontext);

      ////= __allocate
      ////   descriptor_pool::Builder(m_pcontext)
      ////   .setMaxSets(swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, swap_chain_render_pass::MAX_FRAMES_IN_FLIGHT)
      ////   .build();
      //loadGameObjects();

      ////m_pcontext = __allocate context(m_pvulkandevice);

      //::pointer_array<buffer> uboBuffers;

      //uboBuffers.set_size(render_pass::MAX_FRAMES_IN_FLIGHT);

      //for (int i = 0; i < uboBuffers.size(); i++) 
      //{

      //   uboBuffers[i] = __allocate buffer();

      //   uboBuffers[i]->initialize_buffer(
      //      m_pcontext,
      //      sizeof(GlobalUbo),
      //      1,
      //      VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
      //      VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);

      //   uboBuffers[i]->map();

      //}
      //auto globalSetLayout = set_descriptor_layout::Builder(m_pcontext)
      //   .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
      //   .build();


      //std::vector<VkDescriptorSet> globalDescriptorSets(render_pass::MAX_FRAMES_IN_FLIGHT);

      //for (int i = 0; i < globalDescriptorSets.size(); i++)
      //{

      //   auto bufferInfo = uboBuffers[i]->descriptorInfo();

      //   descriptor_writer(*globalSetLayout, *m_pglobalpool)
      //      .writeBuffer(0, &bufferInfo)
      //      .build(globalDescriptorSets[i]);

      //}

      //SimpleRenderSystem simpleRenderSystem{
      //    m_pcontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout() };

      //PointLightSystem pointLightSystem{
      //    m_pcontext,
      //    m_prenderer->getRenderPass(),
      //    globalSetLayout->getDescriptorSetLayout()
      //};

      ////camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      //auto viewerObject = application_object::createGameObject();
      //m_pcontainer->m_bLastMouse = true;
      //viewerObject.transform.translation.z = -2.5f;
      //MNKController cameraController{};
      ///*    glfwSetInputMode(_window.getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
      //    glfwSetWindowUserPointer(_window.getGLFWwindow(), &cameraController);*/
      //cameraController.m_bMouseAbsolute;

      //papp->datastream()->get_block("camera", as_memory_block(camera));
      //papp->datastream()->get_block("transform", as_memory_block(viewerObject.transform));
      //papp->datastream()->get_block("camera_controller", as_memory_block(cameraController));

      //auto currentTime = std::chrono::high_resolution_clock::now();
      ////while (!_window.shouldClose())
      //while (!m_pcontainer->m_bShouldClose && task_get_run()) 
      //{

      //   task_iteration();
      //   //glfwPollEvents();

      //   auto newTime = std::chrono::high_resolution_clock::now();
      //   
      //   float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();

      //   currentTime = newTime;

      //   cameraController.handleMouseInput(m_pcontainer);
      //   
      //   cameraController.updateLook(m_pcontainer, cameraController.getX(), cameraController.getY(), viewerObject);
      //   
      //   cameraController.updateMovement(m_pcontainer, frameTime, viewerObject);

      //   //cameraController.moveInPlaneXZ(m_pcontainer, frameTime, viewerObject);

      //   camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

      //   if (m_prenderer->m_pvkcrenderpass->width() > 0
      //      && m_prenderer->m_pvkcrenderpass->height() > 0)
      //   {
      //      
      //      float aspect = m_prenderer->getAspectRatio();
      //      
      //      camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);

      //      if (auto commandBuffer = m_prenderer->beginFrame())
      //      {

      //         int frameIndex = m_prenderer->getFrameIndex();

      //         FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], m_gameObjects };

      //         // update
      //         GlobalUbo ubo{};
      //         ubo.projection = camera.getProjection();
      //         ubo.view = camera.getView();
      //         ubo.inverseView = camera.getInverseView();
      //         pointLightSystem.update(frameInfo, ubo);
      //         uboBuffers[frameIndex]->writeToBuffer(&ubo);
      //         uboBuffers[frameIndex]->flush();

      //         // render
      //         m_prenderer->beginRenderPass(commandBuffer);

      //         simpleRenderSystem.renderGameObjects(frameInfo);
      //         pointLightSystem.render(frameInfo);

      //         m_prenderer->endRenderPass(commandBuffer);
      //         m_prenderer->endFrame();

      //      }

      //   }

      //}

      //papp->datastream()->set("camera_controller", as_memory_block(cameraController));
      //papp->datastream()->set("transform", as_memory_block(viewerObject.transform));
      //papp->datastream()->set("camera", as_memory_block(camera));

      //if (m_pcontext->logicalDevice() != VK_NULL_HANDLE) 
      //{

      //   vkDeviceWaitIdle(m_pcontext->logicalDevice());

      //}

   }


   void application_handler::resize(int cx, int cy)
   {

      m_p3dapplication->resize(cx, cy);
      //m_prenderer->m_pvkcrenderpass->windowExtent.width = cx;
      //m_prenderer->m_pvkcrenderpass->windowExtent.height = cy;

   }


   void application_handler::load_game_objects()
   {


      m_p3dapplication->load_game_objects();
      //{
      //   ::pointer<model> vkcModel = model::createModelFromFile(m_pcontext, "matter://models/flat_vase.obj");
      //   auto flatVase = application_object::createGameObject();

      //   flatVase.model = vkcModel;
      //   flatVase.transform.translation = { -.5f, .5f, 0.f };
      //   flatVase.transform.scale = { 3.f, 1.5f, 3.f };
      //   m_gameObjects.emplace(flatVase.getId(), std::move(flatVase));
      //}

      //{
      //   ::pointer<model> vkcModel = model::createModelFromFile(m_pcontext, "matter://models/quad.obj");
      //   auto floor = application_object::createGameObject();
      //   floor.model = vkcModel;
      //   assert(vkcModel);
      //   floor.transform.translation = { 0.f, .5f, 0.f };
      //   floor.transform.scale = { 8.f, 1.f, 8.f };
      //   m_gameObjects.emplace(floor.getId(), std::move(floor));
      //}

      //{
      //   ::pointer<model> vkcModel = model::createModelFromFile(m_pcontext, "matter://models/smooth_vase.obj");
      //   auto smoothVase = application_object::createGameObject();
      //   smoothVase.model = vkcModel;
      //   smoothVase.transform.translation = { .5f, .5f, 0.f };
      //   smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
      //   m_gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

      //}

      //{
      //   ::pointer<model> vkcModel = model::createModelFromFile(m_pcontext, "matter://models/StoneSphere.obj");
      //   auto stoneSphere = application_object::createGameObject();
      //   stoneSphere.model = vkcModel;
      //   stoneSphere.transform.translation = { .0f, 0.f, 2.f };
      //   stoneSphere.transform.scale = { .5f, .5f, .5f };
      //   m_gameObjects.emplace(stoneSphere.getId(), std::move(stoneSphere));

      //}

      //{

      //   ::pointer<model> vkcModel = model::createModelFromFile(m_pcontext, "matter://models/Barrel_OBJ.obj");
      //   auto woodBarrel = application_object::createGameObject();
      //   woodBarrel.model = vkcModel;
      //   woodBarrel.transform.translation = {1.f, -.4f, -1.5f };
      //   woodBarrel.transform.scale = { 1.f, 1.f, 1.f };
      //   m_gameObjects.emplace(woodBarrel.getId(), std::move(woodBarrel));

      //}

      //float fLo = 0.5f;

      //std::vector<glm::vec3> lightColors{
      //      {1.f, fLo, fLo},
      //      {fLo, fLo, 1.f},
      //      {fLo, 1.f, fLo},
      //      {1.f, 1.f, fLo},
      //      {fLo, 1.f, 1.f},
      //      {1.f, 1.f, 1.f}  
      //};

      //for (int i = 0; i < lightColors.size(); i++) {
      //   auto pointLight = application_object::makePointLight(0.2f);
      //   pointLight.color = lightColors[i];
      //   auto rotateLight = glm::rotate(
      //      glm::mat4(1.f),
      //      (i * glm::two_pi<float>()) / lightColors.size(),
      //      { 0.f, -1.f, 0.f });
      //   pointLight.pointLight->lightIntensity = 1.0f;
      //   pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
      //   m_gameObjects.emplace(pointLight.getId(), std::move(pointLight));
      //}

   }


} // namespace graphics3d_vulkan 


