// Created by camilo on 2023-11-06 08:07 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "child_frame.h"


namespace app_core_vulken
{


   child_frame::child_frame()
   {



      //window_enable_full_screen();


   }


   child_frame::~child_frame()
   {

   }


   //void child_frame::assert_ok() const
   //{

   //   simple_frame_window::assert_ok();

   //}


   //void child_frame::dump(dump_context & dumpcontext) const
   //{

   //   simple_frame_window::dump(dumpcontext);

   //}



   void child_frame::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::simple_child_frame::_001OnNcDraw(pgraphics);

   }


   void child_frame::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::simple_child_frame::_001OnDraw(pgraphics);

   }



} // namespace app_core_vulken







