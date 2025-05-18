#pragma once

//#include "vulkan/application.h"
#include "app-cube/cube/platform/application.h"
#include "app-cube/cube/application.h"


namespace vulkan_graphics3d_landen
{


   class CLASS_DECL_VULKAN_GRAPHICS3D_LANDEN application :
      virtual public ::cube::application
   {
   public:


      //::user::document *                     m_pdocMenu;
      //::user::single_document_template *     m_ptemplateFuelMain;
      //::user::single_document_template *     m_ptemplateFuelImpact;
      //::pointer < ::cube::application > m_p3dapplication;

      application();
      ~application() override;
      
      __DECLARE_APPLICATION_RELEASE_TIME();



      void init_instance() override;
      void term_instance() override;

      void on_request(::request * prequest) override;


      long long increment_reference_count() override;
      long long decrement_reference_count() override;

      
      //string draw2d_get_default_implementation_name() override;

      
      virtual void on_layout_cube(int cx, int cy);


   };


} // namespace vulkan_graphics3d_landen



