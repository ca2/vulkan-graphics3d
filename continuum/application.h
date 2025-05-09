#pragma once


#include "lowland/landen/application.h"


namespace lowland_continuum
{


   class CLASS_DECL_lowland_continuum application :
      virtual public lowland_landen::application
   {
   public:


      ::file::path                                    m_pathApplicationText;
      ::user::document *                              m_pdocMenu;
      //::pointer<::user::single_document_template>  m_ptemplateSimpleDrawingMain;
      //pane_impact *                                     m_ppaneimpact;

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


} // namespace lowland_continuum



