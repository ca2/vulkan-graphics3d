#include "framework.h"
#include "application.h"
#include "main_frame.h"
#include "document.h"
#include "impact.h"
#include "pane_impact.h"
#include "acme/filesystem/filesystem/file_system_options.h"
#include "aura/graphics/write_text/font.h"
#include "acme/handler/request.h"
#include "aura/user/user/plain_edit.h"
#include "aura/user/user/still.h"
#include "apex/database/stream.h"
#include "base/user/user/tab_impact.h"
#include "base/platform/session.h"
#include "base/user/user/document_manager.h"
#include "base/user/user/user.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(vulkan_graphics3d_continuum);
IMPLEMENT_APPLICATION_FACTORY(vulkan_graphics3d_continuum);


namespace vulkan_graphics3d_continuum
{


   application::application()
   {
      m_bAbsoluteMousePosition = false;
      m_ppaneimpact = nullptr;
      //m_bImaging = false;

      //m_bImaging = true; // for showing application icon
      
      //m_bImaging = false; // showing application icon may use innate_ui icon?

      //m_bNetworking = false;

      //m_ppaneimpact = nullptr;

      m_strAppId = "vulkan-graphics3d/continuum";

      m_strAppName = "vulkan-graphics3d/continuum";

      m_strBaseSupportId = "vulkan_graphics3d_continuum";

      m_bLicense = false;
      
      m_pfilesystemoptions->m_b_iCloudContainer = true;
      m_pfilesystemoptions->m_str_iCloudContainerIdentifier = "iCloud.app-simple";


   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      factory()->add_factory_item <::vulkan_graphics3d_continuum::document >();
      factory()->add_factory_item <::vulkan_graphics3d_continuum::main_frame >();
      factory()->add_factory_item <::vulkan_graphics3d_continuum::impact >();
      factory()->add_factory_item <::vulkan_graphics3d_continuum::pane_impact >();

      ::core::application::init_instance();

      add_impact_system(
         "main", __initialize_new ::user::single_document_template(
         "main",
         ::type < document >(),
         ::type < main_frame >(),
         ::type < pane_impact >()));
      
      add_impact_system(
         "impact", __initialize_new::user::single_document_template(
            "impact",
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

      //if (is_true("wfi_maximize"))
      //{

      //   prequest->payload("document").cast < document >()->get_typed_impact < ::user::tab_impact >()->top_level_frame()->design_window_maximize();

      //}

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


   void application::create_options_body(::user::interaction* pparent)
   {

      auto pstillTitle = create_label<::user::still>(pparent, "vulkan-land continuum Options");

      __defer_construct(pstillTitle->m_pfont);

      pstillTitle->m_pfont->create_font(e_font_sans_ui, 24_pt);

      auto playoutLine = create_line_layout(pparent, e_orientation_horizontal);

      {

         auto pcheckbox = create_check_box<::user::check_box>(playoutLine, "");

         bool bAbsoluteMousePosition = false;

         if (datastream()->get("Absolute Mouse Position", bAbsoluteMousePosition))
         {

            pcheckbox->set_check(bAbsoluteMousePosition, ::e_source_initialize);

         }

         m_bAbsoluteMousePosition = bAbsoluteMousePosition;

         pcheckbox->check_changed(this) += [this](auto& check)
            {

               auto bAbsoluteMousePosition = check.payload().as_bool();

               datastream()->set("Absolute Mouse Position", bAbsoluteMousePosition);


            bAbsoluteMousePosition = false;
            datastream()->get("Absolute Mouse Position", bAbsoluteMousePosition);

            m_bAbsoluteMousePosition = bAbsoluteMousePosition;

               //on_change_synchronize_with_weather();

            };

         create_label<::user::still>(playoutLine, "Absolute Mouse Position");

      }

      //{

      //   auto pcomponent = system()->component("wallpaper_transform");

      //   pcomponent->_001CreateForm(playoutLine);

      //}

      //m_pyeetask->m_pyee->create_yee_options_impact(pparent);

      //create_label<::user::still>(pparent, "yee color");

      //m_peditYeeColor = create_edit<::user::plain_edit>(pparent);

      //m_peditYeeColor->set_size({ 300, 25 });

      //m_peditYeeColor->add_handler(this);

      //::string strYeeColor;

      //datastream()->get("yee_color", strYeeColor);

      //m_peditYeeColor->set_text(strYeeColor, ::e_source_initialize);

      //auto pstill = __allocate ::user::still();

      //__construct_new(m_pedit);

      //__construct_new(m_pbuttonClear);

      //__construct_new(m_pbuttonSend);

      //__construct_new(m_pstillReceiver);

      //m_pstill->create_control(this, "still");

      //m_pedit->create_control(this, "edit");

      //m_pedit->add_handler(this);

      //m_pbuttonClear->create_control(this, "clear_button");

      //m_pbuttonClear->add_handler(this);

      //m_pbuttonSend->create_control(this, "send_button");

      //m_pbuttonSend->add_handler(this);

      //m_pstill->set_window_text("Enter ___new text:");

      //m_pstillReceiver->create_control(this, "still");

      //m_pstillReceiver->set_window_text("(Waiting to receive...)");

      //m_pedit->m_strEmtpyText = "Enter New Text Here";

      //string strInitialText;

      //auto papp = get_app();

      //papp->datastream()->get("last_text", strInitialText);

      //m_pedit->set_text(strInitialText, ::e_source_initialize);

      //m_pbuttonClear->set_window_text("Clear");

      //m_pbuttonSend->set_window_text("Send");

   }


   bool application::is_absolute_mouse_position()
   {

      return m_bAbsoluteMousePosition;

   }
   ::string application::graphics3d_get_implementation_name()
   {

      return "vulkan";
   }

} // namespace vulkan_graphics3d_continuum



