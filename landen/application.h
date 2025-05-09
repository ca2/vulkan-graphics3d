#pragma once

#include "vulkan/application.h"
#include "core/platform/application.h"


namespace lowland_landen
{


   class CLASS_DECL_LOWLAND_LANDEN application :
      virtual public ::core::application
   {
   public:


      ::user::document *                     m_pdocMenu;
      //::user::single_document_template *     m_ptemplateFuelMain;
      //::user::single_document_template *     m_ptemplateFuelImpact;
      ::pointer<impact> m_pimpact;
      ::task_pointer m_ptask3dApp;
      ::pointer < ::vulkan::application > m_pvulkanapplication;

      application();
      ~application() override;
      
      __DECLARE_APPLICATION_RELEASE_TIME();



      void init_instance() override;
      void term_instance() override;

      void on_request(::request * prequest) override;


      long long increment_reference_count() override;
      long long decrement_reference_count() override;

      
      string draw2d_get_default_implementation_name() override;

      
      virtual void update_3d_application(int cx, int cy);


   };


} // namespace lowland_landen



