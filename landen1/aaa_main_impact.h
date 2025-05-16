#pragma once


#include "base/user/user/split_impact.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN main_impact:
      virtual public ::user::split_impact
   {
   public:


      ::pointer < ::app_core_vulken::top_impact >  m_ptopimpact;
      ::pointer < ::app_core_vulken::impact >      m_pimpact;


      main_impact();
      virtual ~main_impact();


      // void assert_ok() const override;

      // void dump(dump_context & dumpcontext) const override;

      void on_create_split_impact();

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      //virtual void handle(::topic * ptopic, ::handler_context * phandlercontext);

      void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

   };


} // namespace app_core_vulken





