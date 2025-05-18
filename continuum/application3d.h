// From continuum (V0idsEmbrace@Twitch)
// by camilo on 2025-05-07 02:18 <3ThomasBorregaardSorensen!!
#pragma once


#include "app-cube/cube/graphics3d/application.h"


namespace vulkan_graphics3d_continuum
{


   class application3d :
      virtual public ::graphics3d::application
   {
   public:



      application3d();
      ~application3d() override;


      ::graphics3d::camera get_default_camera() override;


      void load_game_objects() override;


   };


} // namespace vulkan_graphics3d_continuum



