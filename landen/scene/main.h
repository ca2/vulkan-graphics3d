// Created by camilo on 2025-05-17 04:41 <3ThomasBorregaardSorensen!!
#pragma once


#include "app-cube/cube/graphics3d/scene.h"


namespace vulkan_graphics3d_landen
{


   class main_scene :
      virtual public ::graphics3d::scene
   {
   public:



      main_scene();
      ~main_scene() override;

      void on_load_scene(::gpu::context* pcontext) override;


   };


}//    namespace vulkan_graphics3d_landen



