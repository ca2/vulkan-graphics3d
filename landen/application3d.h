// Created by camilo on 2025-05-17 04:41 <3ThomasBorregaardSorensen!!
#pragma once


#include "app-cube/cube/application.h"


namespace vulkan_graphics3d_landen
{


   class application3d :
      virtual public ::cube::application
   {
   public:



      application3d();
      ~application3d() override;

      void load_game_objects() override;


   };


}//    namespace vulkan_graphics3d_landen



