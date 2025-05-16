// Created by camilo on 2024-07-27 <3ThomasBorregaardSorensen!!
#pragma once


namespace vulkan
{


   class CLASS_DECL_VULKAN_LAND_LANDEN application :
      virtual public ::particle
   {
   public:

      ::function < void(void*, int, int, int)> m_callbackOffscreen;


      application();
      ~application() override;


      virtual void handleMouseMove(int x, int y);

      virtual void run_application();

      virtual void resize(int cx, int cy);



   };



} // namespace vulkan


