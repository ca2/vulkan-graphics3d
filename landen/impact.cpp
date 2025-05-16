#include "framework.h"
#include "impact.h"
#include "application.h"
#include "acme/constant/message.h"
#include "acme/graphics/image/image32.h"
#include "acme/handler/topic.h"
#include "acme/platform/node.h"
#include "acme/platform/session.h"
#include "aura/graphics/draw2d/graphics.h"
#include "aura/graphics/draw2d/pen.h"
#include "aura/graphics/image/context.h"
#include "aura/graphics/image/image.h"
#include "aura/graphics/image/drawing.h"
#include "aura/message/user.h"
#include "vulkan/vk_container.h"


int run_vulkan_example();

int run_vulkan_example2(const ::function < void(void*, int, int, int)>& callback);

int run_vulkan_example5(mouseState* pmousestate, const ::function < void(void*, int, int, int)>& callback);


namespace vulkan
{


   ::pointer<::vulkan::application > start_offscreen_application(vkc::VkContainer* pvkcontainer, mouseState* pmousestate);


} // namespace vulkan





//#include "aura/update.h"
//#include <math.h>
// http://stackoverflow.com/questions/4052940/how-to-make-an-opengl-rendering-context-with-transparent-background
//http://stackoverflow.com/users/176769/karlphillip
//#include <windows.h>
//#ifdef WINDOWS
//#include <windowsx.h>
//#endif

//
//#ifdef LINUX
//
//#define WIDTH 3200
//#define HEIGHT 1800f
//
//const int sbAttrib[] = {GLX_DOUBLEBUFFER, 0, GLX_RED_SIZE, 1,GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,GLX_ALPHA_SIZE, 1, GLX_DEPTH_SIZE, 16,None};
//int pbAttrib[] = {GLX_PBUFFER_WIDTH, WIDTH,GLX_PBUFFER_HEIGHT, HEIGHT,GLX_PRESERVED_CONTENTS, True,None };
//
//#endif
//
//
////extern CLASS_DECL_AXIS thread_int_ptr < DWORD_PTR > t_time1;
//GLfloat LightAmbient[] = { 0.5f, 0.5f, 0.5f, 1.00f };
//GLfloat LightDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.00f };
//GLfloat LightPosition[] = { 0.0f, 0.0f, 2.0f, 1.00f };
////GLfloat LightPosition2[] = { -5.0f, -5.0f, 32.0f, 1.00f };

//#ifdef WINDOWS
//typedef BOOL(WINAPI * PFNWGLDESTROYPBUFFERARBPROC) (HPBUFFERARB hPbuffer);
//typedef BOOL(WINAPI * PFNWGLQUERYPBUFFERARBPROC) (HPBUFFERARB hPbuffer, int iAttribute, int *piValue);
//typedef HDC(WINAPI * PFNWGLGETPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer);
//typedef HPBUFFERARB(WINAPI * PFNWGLCREATEPBUFFERARBPROC) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
//typedef int (WINAPI * PFNWGLRELEASEPBUFFERDCARBPROC) (HPBUFFERARB hPbuffer, HDC hDC);
//
//PFNWGLDESTROYPBUFFERARBPROC                       wglDestroyPbufferARB;
//PFNWGLQUERYPBUFFERARBPROC                         wglQueryPbufferARB;
//PFNWGLGETPBUFFERDCARBPROC                         wglGetPbufferDCARB;
//PFNWGLCREATEPBUFFERARBPROC                        wglCreatePbufferARB;
//PFNWGLRELEASEPBUFFERDCARBPROC                     wglReleasePbufferDCARB;
//
//
//typedef BOOL(WINAPI * PFNWGLCHOOSEPIXELFORMATARBPROC) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, unsigned int nMaxFormats, int *piFormats, unsigned int *nNumFormats);
//typedef BOOL(WINAPI * PFNWGLGETPIXELFORMATATTRIBFVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, unsigned int nAttributes, const int *piAttributes, FLOAT *pfValues);
//typedef BOOL(WINAPI * PFNWGLGETPIXELFORMATATTRIBIVARBPROC) (HDC hdc, int iPixelFormat, int iLayerPlane, unsigned int nAttributes, const int *piAttributes, int *piValues);
//
//PFNWGLCHOOSEPIXELFORMATARBPROC                    wglChoosePixelFormatARB;
//PFNWGLGETPIXELFORMATATTRIBFVARBPROC               wglGetPixelFormatAttribfvARB;
//PFNWGLGETPIXELFORMATATTRIBIVARBPROC               wglGetPixelFormatAttribivARB;
//
//
//#endif

#include <assert.h>
#ifdef WINDOWS
#include <tchar.h>
#endif // WINDOWS
//#ifdef  assert
//#define verify(expr) if(!expr) assert(0)
//#else verify(expr) expr
//#endif






namespace vulkan_land_landen
{



   impact::impact()
   {


   }


   impact::~impact()
   {

   }


   void impact::install_message_routing(::channel* pchannel)
   {

      ::user::impact::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create, pchannel, this, &impact::on_message_create);
      MESSAGE_LINK(e_message_destroy, pchannel, this, &impact::on_message_destroy);
      MESSAGE_LINK(e_message_key_down, pchannel, this, &impact::on_message_key_down);
      MESSAGE_LINK(e_message_key_up, pchannel, this, &impact::on_message_key_up);
      MESSAGE_LINK(e_message_mouse_move, pchannel, this, &impact::on_message_mouse_move);
      MESSAGE_LINK(e_message_left_button_down, pchannel, this, &impact::on_message_left_button_down);
      MESSAGE_LINK(e_message_left_button_up, pchannel, this, &impact::on_message_left_button_up);
      MESSAGE_LINK(e_message_mouse_leave, pchannel, this, &impact::on_message_mouse_leave);

   }


   bool impact::keyboard_focus_is_focusable()
   {

      return true;

   }


   void impact::on_message_key_down(::message::message* pmessage)
   {

      auto pkey = pmessage->m_union.m_pkey;

      pmessage->m_bRet = true;

   }


   void impact::on_message_key_up(::message::message* pmessage)
   {

      auto pkey = pmessage->m_union.m_pkey;

      pmessage->m_bRet = true;

   }


   void impact::on_message_mouse_move(::message::message* pmessage)
   {

      auto pmouse = pmessage->m_union.m_pmouse;

      pmessage->m_bRet = true;

      if (get_app()->m_pvulkanapplication)
      {

         auto point = pmouse->m_pointHost;

         host_to_client()(point);

         //m_mousestate.position.x = point.x();
         //m_mousestate.position.y = point.y();
         //m_mousestate.buttons.left = true;
         //         pmouse->m_p

         double w = m_width;
         double h = m_height;

         if (m_bLastMouse)
         {

            m_bLastMouse = false;
            m_bFirstMouse = true;

         }

         double xCursor;
         double yCursor;

         if (is_absolute_mouse_position())
         {

            m_dCursorX = ((point.x() - (w / 2.0)) * 2.0);
            m_dCursorY = ((point.y() - (h / 2.0)) * 2.0);

         }
         else
         {

            //xCursor = point.x();
            //yCursor = point.y();

            m_dCursorX = point.x();
            m_dCursorY = point.y();

         }

         track_mouse_leave();

         //get_app()->m_pvulkanapplication->handleMouseMove(xCursor, yCursor);

      }

   }


   void impact::on_message_mouse_leave(::message::message* pmessage)
   {

      reset_mouse_last_position();

   }


   void impact::reset_mouse_last_position()
   {

      if (is_absolute_mouse_position())
      {

         m_dCursorX = 0.;
         m_dCursorY = 0.;

      }

      m_bLastMouse = true;

   }


   void impact::on_message_left_button_down(::message::message* pmessage)
   {

      auto pmouse = pmessage->m_union.m_pmouse;

      pmessage->m_bRet = true;

      auto point = pmouse->m_pointHost;

      host_to_client()(point);

      m_mousestate.position.x = point.x();
      m_mousestate.position.y = point.y();
      m_mousestate.buttons.left = true;

   }


   void impact::on_message_left_button_up(::message::message* pmessage)
   {

      auto pmouse = pmessage->m_union.m_pmouse;

      pmessage->m_bRet = true;

      auto point = pmouse->m_pointHost;

      host_to_client()(point);

      m_mousestate.position.x = point.x();
      m_mousestate.position.y = point.y();
      m_mousestate.buttons.left = false;

   }


   ::pointer < ::vulkan::application > impact::start_vulkan_application()
   {

      return ::vulkan::start_offscreen_application(this, &m_mousestate);

   }


   ::int_size impact::size()
   {

      return this->raw_rectangle().size();

   }


   void impact::on_message_create(::message::message* pmessage)
   {

      ::pointer<::message::create>pcreate(pmessage);

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }

      get_app()->m_pimpact = this;

      m_pparticleImageSynchronization = node()->create_mutex();

      m_pimage = image()->create_image(int_size{ 1920, 1080 });

   }


   void impact::on_message_destroy(::message::message* pmessage)
   {

   }


   void impact::on_timer(timer* ptimer)
   {

   }


   void impact::handle(::topic* ptopic, ::handler_context* phandlercontext)
   {

      if (ptopic->id() == id_initial_update)
      {
         
         //::pointer<::userex::pane_tab_impact>ppaneimpact = get_typed_parent < ::userex::pane_tab_impact >();
         ////if(ppaneimpact.is_set())
         ////{
         ////   ppaneimpact->m_pflagview = this;
         ////}

      }

      __UNREFERENCED_PARAMETER(ptopic);

   }


   void impact::_001OnNcClip(::draw2d::graphics_pointer& pgraphics)
   {

      ::user::impact::_001OnNcClip(pgraphics);

   }


   void impact::_001OnClip(::draw2d::graphics_pointer& pgraphics)
   {

      ::user::impact::_001OnClip(pgraphics);

   }


   void impact::_001OnNcDraw(::draw2d::graphics_pointer& pgraphics)
   {

      ::user::impact::_001OnNcDraw(pgraphics);

   }


   void impact::_001OnDraw(::draw2d::graphics_pointer& pgraphics)
   {

      ////return;

      //pgraphics->fill_solid_rectangle(::int_rectangle_dimension(10, 10, 100, 100), argb(128, 200, 220, 255));

      ////return;

      //pgraphics->fill_solid_rectangle(::int_rectangle_dimension(110, 100, 100, 100), argb(255, 200, 220, 255));

      //auto ppen = __øcreate < ::draw2d::pen > ();

      //ppen->create_solid(10, argb(200, 20, 155, 50));

      //pgraphics->set(ppen);

      //pgraphics->draw_line(::double_point( 0.0, 0.0 ), ::double_point(300.0, 300.0));

      //pgraphics->fill_solid_rectangle(::int_rectangle_dimension(10, 150, 100, 100), argb(128, 100, 255, 120));

      //pgraphics->fill_solid_rectangle(::int_rectangle_dimension(110, 150, 100, 100), argb(255, 100, 255, 120));

      //auto ppen2 = __øcreate < ::draw2d::pen > ();

      //ppen2->create_solid(5, argb(200, 20, 155, 50));

      //pgraphics->set(ppen2);

      //pgraphics->draw_line(::double_point(300.0, 0.0), ::double_point(100.0, 300.0));

      //pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_set);

      if (::is_ok(m_pimage))
      {

         pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);

         _synchronous_lock synchronouslock(m_pparticleImageSynchronization);

         ::image::image_source imagesource(m_pimage, m_pimage->rectangle());

         ::image::image_drawing_options imagedrawingoptions(m_pimage->rectangle());

         ::image::image_drawing imagedrawing(imagedrawingoptions, imagesource);

         pgraphics->draw(imagedrawing);

      }

   }


   void impact::_001OnNcPostDraw(::draw2d::graphics_pointer& pgraphics)
   {


   }


   ::user::document* impact::get_document()
   {

      return  (::user::impact::get_document());

   }


   ::user::enum_key_state impact::get_key_state(::user::e_key ekey)
   {

      if (session()->is_key_pressed(ekey))
      {

         return ::user::e_key_state_pressed;

      }

      return ::user::e_key_state_none;

   }


   void impact::on_layout(::draw2d::graphics_pointer& pgraphics)
   {

      auto rectangleX = this->rectangle();

      if (rectangleX.area() <= 0)
      {

         return;

      }

      m_width = rectangleX.width();
      m_height = rectangleX.height();

      get_app()->update_3d_application(m_width, m_height);

      reset_mouse_last_position();

   }


} // namespace app_core_vulken



