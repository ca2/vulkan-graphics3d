// Created by camilo on 2023-11-06 08:05 <3ThomasBorregaardSorensen!!
#pragma once


#include "base/user/simple/child_frame.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN child_frame :
      virtual public simple_child_frame
   {
   public:


      child_frame();
      ~child_frame() override;


      // void assert_ok() const override;
      // void dump(dump_context & dumpcontext) const override;

      void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;


   };


} // namespace app_core_vulken



