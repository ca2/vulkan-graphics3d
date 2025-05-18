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
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/drawing.h"
#include "base/user/user/single_document_template.h"


__IMPLEMENT_APPLICATION_RELEASE_TIME(vulkan_graphics3d_landen);
IMPLEMENT_APPLICATION_FACTORY(vulkan_graphics3d_landen);


namespace vulkan_graphics3d_landen
{


   // __IMPLEMENT_APPLICATION_RELEASE_TIME1(app_core_vulken));


   application::application()
   {


      //m_ptemplateFuelMain = nullptr;
      //m_ptemplateFuelImpact = nullptr;

      m_strAppId              = "vulkan-cube/landen";
      m_strAppName            = "vulkan-cube/landen";
      m_strBaseSupportId      = "vulkan-cube/landen";
      m_bLicense              = false;

   }


   application::~application()
   {

   }


   void application::init_instance()
   {

      //factory()->add_factory_item <::app_core_vulken::child_frame >();
      factory()->add_factory_item <::vulkan_graphics3d_landen::document >();
      factory()->add_factory_item <::vulkan_graphics3d_landen::frame >();
      //factory()->add_factory_item <::app_core_vulken::edit_impact >();
      factory()->add_factory_item <::vulkan_graphics3d_landen::impact >();
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
                          ::type < ::vulkan_graphics3d_landen::document >(),
                          ::type < ::vulkan_graphics3d_landen::frame >(),       // top level SDI frame::windowing::window
                        ::type < ::vulkan_graphics3d_landen::impact >()));
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


   //void application::update_3d_application(int cx, int cy)
   //{

   //   if (!m_p3dapplication)
   //   {

   //      if (!m_pimpact->m_callbackOffscreen)
   //      {

   //         m_pimpact->m_callbackOffscreen = [this](void* p, int w, int h, int stride)
   //            {

   //               {

   //                  _synchronous_lock synchronouslock(m_pimpact->m_pparticleImageSynchronization);

   //                  m_pimpact->m_pimage->image32()->copy(m_pimpact->m_pimage->size().minimum(::int_size(w, h)), m_pimpact->m_pimage->m_iScan, (image32_t*)p, stride);

   //                  for (int y = 0; y < h; y++)
   //                  {

   //                     auto p = (unsigned char*)(m_pimpact->m_pimage->image32() + (y * m_pimpact->m_pimage->m_iScan) / 4);

   //                     for (int x = 0; x < w; x++)
   //                     {

   //                        //p[0] = p[0] * p[3] / 255;
   //                        //p[1] = p[1] * p[3] / 255;
   //                        //p[2] = p[2] * p[3] / 255;

   //                        auto r = p[0];
   //                        auto g = p[1];
   //                        auto b = p[2];
   //                        auto a = p[3];
   //                        p[0] = b;
   //                        p[2] = r;
   //                        //p[3] = 255;

   //                        /*         if (r > a)
   //                                 {

   //                                    information("What a red!!"_ansi);

   //                                 }

   //                                 if (g > a)
   //                                 {

   //                                    information("What a green!!"_ansi);

   //                                 }

   //                                 if (b > a)
   //                                 {

   //                                    information("What a blue!!"_ansi);

   //                                 }*/

   //                        p += 4;

   //                     }

   //                  }

   //               }


   //               m_pimpact->set_need_redraw();
   //               m_pimpact->post_redraw();
   //            };

   //      }

   //      m_ptask3dApp = fork([this]()
   //         {

   //            //            run_vulkan_example();

   //            m_p3dapplication = m_pimpact->start_graphics3d_application();

   //            m_p3dapplication->m_callbackOffscreen = m_pimpact->m_callbackOffscreen;

   //            m_p3dapplication->run_application();

   //            m_ptask3dApp.release();

   //         });




   //   }
   //   else
   //   {

   //      m_p3dapplication->resize(cx, cy);

   //   }



   //}


   void application::term_instance()
   {
      //m_ptask3dApp->set_finish();
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


//   ::string application::draw2d_get_default_implementation_name()
  // {

    //  return system()->implementation_name("draw2d"_ansi, "gdiplus"_ansi);

   //}


   //__namespace_application_factory("app-core/vulken"_ansi);

   ::string application::graphics3d_get_implementation_name()
   {

      return "vulkan";
   }
} // namespace vulkan_graphics3d_landen







