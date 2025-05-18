// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "application3d.h"
#include "app-cube/cube/graphics3d/application_object.h"


namespace vulkan_graphics3d_continuum
{


   application3d::application3d()
   {

   }


   application3d::~application3d()
   {

   }


   ::graphics3d::camera application3d::get_default_camera()
   {

      glm::vec3 cameraPos = glm::vec3(0.0f, .0f, -5.0f);
      glm::vec3 target = glm::vec3(0.0f); // Look at origin
      glm::vec3 direction = glm::normalize(target - cameraPos);
      //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      return {cameraPos, target, direction };


   }


   void application3d::load_game_objects()
   {

      {

         auto pmodel = createModelFromFile(m_pcontext, "matter://models/flat_vase.obj");
         auto flatVase = __øcreate < ::graphics3d::application_object >();
         flatVase->m_pmodel = pmodel;
         flatVase->m_transform.translation = { -.5f, .5f, 0.f };
         flatVase->m_transform.scale = { 3.f, 1.5f, 3.f };
         add_object(flatVase);

      }

      {

         auto pmodel = createModelFromFile(m_pcontext, "matter://models/quad.obj");
         auto floor = __øcreate < ::graphics3d::application_object >();
         floor->m_pmodel = pmodel;
         assert(pmodel);
         floor->m_transform.translation = { 0.f, .5f, 0.f };
         floor->m_transform.scale = { 8.f, 1.f, 8.f };
         add_object(floor);

      }

      {

         auto pmodel = createModelFromFile(m_pcontext, "matter://models/smooth_vase.obj");
         auto smoothVase = __øcreate < ::graphics3d::application_object >();
         smoothVase->m_pmodel = pmodel;
         smoothVase->m_transform.translation = { .5f, .5f, 0.f };
         smoothVase->m_transform.scale = { 3.f, 1.5f, 3.f };
         add_object(smoothVase);

      }

      {
         auto pmodel = createModelFromFile(m_pcontext, "matter://models/StoneSphere.obj");
         auto stoneSphere = __øcreate < ::graphics3d::application_object >();
         stoneSphere->m_pmodel = pmodel;
         stoneSphere->m_transform.translation = { .0f, 0.f, 2.f };
         stoneSphere->m_transform.scale = { .5f, .5f, .5f };
         add_object(stoneSphere);

      }

      {

         auto pmodel = createModelFromFile(m_pcontext, "matter://models/Barrel_OBJ.obj");
         auto woodBarrel = __øcreate < ::graphics3d::application_object >();
         woodBarrel->m_pmodel = pmodel;
         woodBarrel->m_transform.translation = { 1.f, -.4f, -1.5f };
         woodBarrel->m_transform.scale = { 1.f, 1.f, 1.f };
         add_object(woodBarrel);

      }

      float fLo = 0.5f;

      std::vector<glm::vec3> lightColors{
            {1.f, fLo, fLo},
            {fLo, fLo, 1.f},
            {fLo, 1.f, fLo},
            {1.f, 1.f, fLo},
            {fLo, 1.f, 1.f},
            {1.f, 1.f, 1.f}
      };

      for (int i = 0; i < lightColors.size(); i++) 
      {
         auto pointLight = __allocate ::graphics3d::point_light (0.2f);
         pointLight->m_color = lightColors[i];
         auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, -1.f, 0.f });
         pointLight->m_pointlightcomponent.lightIntensity = 1.0f;
         pointLight->m_transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
         add_object(pointLight);

      }

   }



} // namespace vulkan_graphics3d_continuum



