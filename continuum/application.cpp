#include "framework.h"
#include "application.h"
#include "main_frame.h"
#include "document.h"
#include "impact.h"
#include "acme/filesystem/filesystem/file_system_options.h"
#include "acme/handler/request.h"
#include "base/user/user/tab_impact.h"
#include "base/platform/session.h"
#include "base/user/user/document_manager.h"
#include "base/user/user/user.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(app_simple_application);
IMPLEMENT_APPLICATION_FACTORY(app_simple_application);


namespace app_simple_application
{


   application::application()
   {

      //m_bImaging = false;

      //m_bImaging = true; // for showing application icon
      
      m_bImaging = false; // showing application icon may use innate_ui icon?

      m_bNetworking = false;

      m_ppaneimpact = nullptr;

      m_strAppId = "app-simple/application";

      m_strAppName = "app-simple/application";

      m_strBaseSupportId = "app_simple_application";

      m_bLicense = false;
      
      m_pfilesystemoptions->m_b_iCloudContainer = true;
      m_pfilesystemoptions->m_str_iCloudContainerIdentifier = "iCloud.app-simple";


   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      factory()->add_factory_item <::app_simple_application::document >();
      factory()->add_factory_item <::app_simple_application::main_frame >();
      factory()->add_factory_item <::app_simple_application::impact >();

      ::base::application::init_instance();

      add_impact_system(
         "main", __initialize_new ::user::single_document_template(
         "main",
         ::type < document >(),
         ::type < main_frame >(),
         ::type < impact >()));
      
      
#if defined(APPLE_IOS)

      m_pathApplicationText = "icloud://iCloud.app-simple/Documents/application/application.txt";
      
#else
      
      m_pathApplicationText = "dropbox-app://application.txt";
      
#endif



   }


   void application::term_application()
   {

      ::base::application::term_application();

   }


   void application::on_request(::request * prequest)
   {

      if (impact_system("main")->get_document_count() == 0)
      {

         if(prequest->m_egraphicsoutputpurpose & ::graphics::e_output_purpose_screen)
         {

            information() << "::graphics::e_output_purpose_screen";

         }
         else
         {

            information() << "NOT ::graphics::e_output_purpose_screen";

         }

         impact_system("main")->request(prequest);

      }

      if (is_true("wfi_maximize"))
      {

         prequest->payload("document").cast < document >()->get_typed_impact < ::user::tab_impact >()->top_level_frame()->design_window_maximize();

      }

      informationf("finished simple_application::on_request");

   }


   string application::preferred_experience()
   {

      return ::aura::application::preferred_experience();

   }


#ifdef _DEBUG


   long long application::increment_reference_count()
   {

      return ::object::increment_reference_count();

   }


   long long application::decrement_reference_count()
   {

      return ::object::decrement_reference_count();

   }


#endif


} // namespace app_simple_application



