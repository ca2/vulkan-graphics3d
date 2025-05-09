#include "framework.h"
#include "frame.h"
#include <math.h>


namespace lowland_landen
{


   frame::frame()
   {



      window_enable_full_screen();


   }


   frame::~frame()
   {

   }


   //void frame::assert_ok() const
   //{

   //   simple_frame_window::assert_ok();

   //}


   //void frame::dump(dump_context & dumpcontext) const
   //{

   //   simple_frame_window::dump(dumpcontext);

   //}


   ::pointer < ::experience::frame > frame::frame_experience()
   {

      auto pframe = ::simple_frame_window::frame_experience(nullptr,"022"_ansi, "LightBlue"_ansi);

      return pframe;

   }


   void frame::_001OnNcClip(::draw2d::graphics_pointer & pgraphics)
   {

      ::simple_frame_window::_001OnNcClip(pgraphics);

   }


   void frame::_001OnClip(::draw2d::graphics_pointer & pgraphics)
   {

      ::simple_frame_window::_001OnClip(pgraphics);

   }


   void frame::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::simple_frame_window::_001OnNcDraw(pgraphics);

   }


   void frame::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      //::simple_frame_window::_001OnDraw(pgraphics);

      ::simple_frame_window::_001OnDraw(pgraphics);

   }


   ::user::enum_translucency frame::get_translucency(::user::style* pstyle)
   {

      if(m_bExplicitTranslucency)
      {

         return m_etranslucency;

      }

      //if(m_puserstyle != nullptr)
      //{

      //   if(m_puserstyle->_001GetMainFrameTranslucency(etranslucency))
      //      return true;

      //}
      return ::user::e_translucency_present;

   }


} // namespace app_core_vulken







