#pragma once


#include "base/platform/application.h"


namespace app_simple_application
{


   class CLASS_DECL_APP_SIMPLE_APPLICATION application :
      virtual public ::base::application
   {
   public:


      ::file::path                                    m_pathApplicationText;
      ::user::document *                              m_pdocMenu;
      //::pointer<::user::single_document_template>  m_ptemplateSimpleDrawingMain;
      pane_impact *                                     m_ppaneimpact;

      application();
      ~application() override;
      

      __DECLARE_APPLICATION_RELEASE_TIME();


      virtual string preferred_experience() override;
      virtual void init_instance() override;
      virtual void term_application() override;

      virtual void on_request(::request * prequest) override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif

   };


} // namespace app_simple_application



