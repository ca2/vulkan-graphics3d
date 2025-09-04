// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "main.h"
#include "aura/platform/application.h"
#include "bred/graphics3d/camera.h"
#include "bred/graphics3d/point_light.h"
#include "bred/graphics3d/scene_object.h"


namespace vulkan_graphics3d_continuum
{


   main_scene::main_scene()
   {

   }


   main_scene::~main_scene()
   {

   }


   ::graphics3d::camera * main_scene::get_default_camera()
   {
      glm::vec3 camera = glm::vec3(0.0f, .0f, -5.0f);
      glm::vec3 target = glm::vec3(0.0f); // Look at origin
      //glm::vec3 direction = glm::normalize(target - cameraPos);
      //camera camera{ glm::vec3(0.0f, 2.0f, -15.0f), -90.0f, 0.0f };
      auto pcamera = øcreate< ::graphics3d::camera>();
      //pcamera->initialize_camera(target, camera);
      //pcamera->m_pimpact = m_pimpact;
      return pcamera;


   }


   void main_scene::on_load_scene(::gpu::context * pcontext)
   {

      {

         auto & flatVase = scene_object("matter://models/flat_vase.obj");
         flatVase.translate({ -.5f, .5f, 0.f });
         flatVase.scale({3.f, 1.5f, 3.f});

      }

      {

         auto & floor = scene_object("matter://models/quad.obj");
         floor.translate({0.f, .5f, 0.f});
         floor.scale({8.f, 1.f, 8.f});

      }

      {

         auto & smoothVase = scene_object("matter://models/smooth_vase.obj");
         smoothVase.translate({.5f, .5f, 0.f});
         smoothVase.scale({3.f, 1.5f, 3.f});

      }

      {

         auto & stoneSphere = scene_object("matter://models/StoneSphere.obj");
         stoneSphere.translate({ .0f, 0.f, 2.f });
         stoneSphere.scale({.5f, .5f, .5f});

      }

      {

         auto & woodBarrel = scene_object("matter://models/Barrel_OBJ.obj");
         woodBarrel.translate({ 1.f, -.4f, -1.5f });
         woodBarrel.scale({1.f, 1.f, 1.f});

      }

      float fLo = 0.5f;

      ::array_base < ::color::color > lightColors{
            argb(1.f, 1.f, fLo, fLo),
            argb(1.f, fLo, fLo, 1.f),
            argb(1.f, fLo, 1.f, fLo),
            argb(1.f, 1.f, 1.f, fLo),
            argb(1.f, fLo, 1.f, 1.f),
            argb(1.f, 1.f, 1.f, 1.f)
      };

      for (int i = 0; i < lightColors.size(); i++) 
      {
         auto ppointlight = create_point_light (0.2f);
         ppointlight->m_color = lightColors[i];
         auto rotateLight = glm::rotate(
            glm::mat4(1.f),
            (i * glm::two_pi<float>()) / lightColors.size(),
            { 0.f, -1.f, 0.f });
         ppointlight->m_fLightIntensity = 1.0f;
         ppointlight->transform().m_vec3Position = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
         //add_object(pointLight);
         m_pointlighta.add(ppointlight);

      }

   }



} // namespace vulkan_graphics3d_continuum



