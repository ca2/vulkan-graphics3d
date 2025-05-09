#include "framework.h"
#include "application.h"
//#include "child_frame.h"
#include "document.h"
//#include "edit_impact.h"
#include "frame.h"
#include "impact.h"
//#include "main_impact.h"
//#include "pane_impact.h"
//#include "toggle_impact.h"
//#include "top_impact.h"
//#include "window.h"
#include "acme/handler/request.h"
#include "acme/platform/system.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(lowland_landen);
IMPLEMENT_APPLICATION_FACTORY(lowland_landen);


namespace lowland_landen
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_core_vulken));


   application::application()
   {


      //m_ptemplateFuelMain = nullptr;
      //m_ptemplateFuelImpact = nullptr;

      m_strAppId              = "lowland/landen";
      m_strAppName            = "lowland/landen";
      m_strBaseSupportId      = "lowland/landen";
      m_bLicense              = false;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      //factory()->add_factory_item <::app_core_vulken::child_frame >();
      factory()->add_factory_item <::lowland_landen::document >();
      factory()->add_factory_item <::lowland_landen::frame >();
      //factory()->add_factory_item <::app_core_vulken::edit_impact >();
      factory()->add_factory_item <::lowland_landen::impact >();
      //factory()->add_factory_item <::app_core_vulken::main_impact >();
      //factory()->add_factory_item <::app_core_vulken::pane_impact >();
      //factory()->add_factory_item <::app_core_vulken::top_impact >();
      //factory()->add_factory_item <::app_core_vulken::toggle_impact >();


      ::core::application::init_instance();

      //if (!::aura::application::init_instance())
      //{

      //   return false;

      //}

      ///set_local_data();

      add_impact_system(
         "main"_ansi, __initialize_new ::user::single_document_template(
                          "main"_ansi,
                          ::type < ::lowland_landen::document >(),
                          ::type < ::lowland_landen::frame >(),       // top level SDI frame::windowing::window
                        ::type < ::lowland_landen::impact >()));
      //::type < ::app_core_vulken::pane_impact >()));
      
      //add_document_template(pdocumenttemplate);
      //
      //m_ptemplateFuelMain = pdocumenttemplate;
      
      //add_impact_system(
      //   "impact"_ansi,
      //   __initialize_new ::user::single_document_template(
      //                    "main"_ansi,
      //                    ::type < ::app_core_vulken::document >(),
      //                    ::type < ::app_core_vulken::child_frame >(),       // top level SDI frame::windowing::window
      //                    //::type < ::app_core_vulken::main_impact >()));
      //               ::type < ::app_core_vulken::impact >()));
   
      //add_document_template(pdocumenttemplate);

      //m_ptemplateFuelImpact = pdocumenttemplate;

      //return true;

   }


   void application::term_instance()
   {

      ::aura::application::term_instance();
   }


   void application::on_request(::request * prequest)
   {

      //if (::is_set(prequest))
      //{

      //   string strMode = prequest->payload("mode"_ansi);

      //   if (strMode == "window"_ansi)
      //   {

      //      auto pwindow = __create_new < window >();

      //      //::user::system cs(
      //      //   WS_EX_LAYERED,
      //      //   NULL,
      //      //   "Fuel Lab - OpenGL Testing"_ansi, 
      //      //   WS_VISIBLE,
      //      //   int_rectangle_dimension(100, 100, 900, 300));

      //      //pwindow->create_window_ex(cs);

      //      //pwindow->set_dim(100, 100, 900, 300);

      //      //pwindow->display();

      //      //pwindow->set_need_redraw();

      //      return;

      //   }


      //}


      if(impact_system("main"_ansi)->get_document_count() == 0)
      {

         impact_system("main"_ansi)->request(prequest);

      }

      //if(prequest->m_payloadFile.has_character())
      //{

      //   m_ptemplateFuelImpact->request(prequest);

      //}

   }


   long long application::increment_reference_count()
   {

      return ::object::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::object::decrement_reference_count();

   }


   ::string application::draw2d_get_default_implementation_name()
   {

      return system()->implementation_name("draw2d"_ansi, "gdiplus"_ansi);

   }


   //__namespace_application_factory("app-core/vulken"_ansi);


} // namespace lowland_landen







