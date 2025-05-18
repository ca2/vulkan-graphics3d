// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#pragma once


#include "app-cube/cube/scene.h"


namespace vulkan_graphics3d_continuum
{


   class main_scene :
      virtual public ::cube::scene
   {
   public:



      main_scene();
      ~main_scene() override;


      ::cube::camera get_default_camera() override;


      void on_load_scene() override;


   };


} // namespace vulkan_graphics3d_continuum



