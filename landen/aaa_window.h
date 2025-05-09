#pragma once


#include "aura/user/user/interaction.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN window :
      virtual public ::user::interaction
   {
   public:


      bool                       m_bExplicitTranslucency;
      ::user::enum_translucency     m_etranslucency;
      ::image::image_pointer                      m_pimage;

      window();
      ~window() override;


      // void assert_ok() const override;
      //virtual void dump(dump_context& dumpcontext) const;

      
      void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      //virtual void _001PrintBuffer(::draw2d::graphics_pointer & pgraphics) override;
      //virtual void do_graphics(::draw2d::graphics_pointer & pgraphics) override;
      void _000CallOnDraw(::draw2d::graphics_pointer & pgraphics) override;


   };


} // namespace app_core_vulken












