// Created by camilo on 2024-07-27 <3ThomasBorregaardSorensen!!
#pragma once


namespace vulkan
{

   class CLASS_DECL_vulkan_land_LANDEN application :
      virtual public ::particle
   {
   public:


      virtual void handleMouseMove(int x, int y);


      virtual void run_application();

      virtual void resize(int cx, int cy);

   };



} // namespace vulkan


