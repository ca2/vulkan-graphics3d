#include "framework.h"
#include "window.h"
#include "aura/graphics/draw2d/graphics.h"


namespace app_core_vulken
{


   window::window()
   {


   }


   window::~window()
   {

   }


   //void window::assert_ok() const
   //{

   //   ::user::interaction::assert_ok();

   //}


   //void window::dump(dump_context& dumpcontext) const
   //{

   //   ::user::interaction::dump(dumpcontext);

   //}
   
   void window::_000CallOnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      //_001OnDraw(pgraphics);

      ::user::interaction::_000CallOnDraw(pgraphics);

   }
   

   void window::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      auto s = rectangleX.size();

      auto r1 = rectangleX;

      r1.right() = 300;

      pgraphics->fill_rectangle(r1, argb(155, 255, 168, 230));

      auto r2 = rectangleX;

      r2.left() = r1.right();

      pgraphics->fill_rectangle(r2, argb(155, 155, 255, 230));

      pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);

      auto r3 = rectangleX;

      r3.deflate(100, 100);

      pgraphics->fill_rectangle(r3, argb(155, 155, 155, 155));

      pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_set);

      auto r4 = rectangleX;

      r4.deflate(200, 75);

      pgraphics->fill_rectangle(r4, argb(155, 0, 0, 0));

   }


} // namespace app_core_vulken







