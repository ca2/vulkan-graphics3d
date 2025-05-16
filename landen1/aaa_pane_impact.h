#pragma once


#include "application.h"
#include "apex/platform/app_consumer.h"
#include "core/user/userex/pane_tab_impact.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN pane_impact :
      virtual public ::app_consumer < application, ::userex::pane_tab_impact, ::user::form_callback >
   {
   public:


      //::pointer < ::user::split_impact >       m_ptopviewLast;
      //::pointer < impact >                     m_pimpactLast;

      //::pointer < ::user::document >           m_pdocMenu;
      //::pointer < ::user::plain_edit_impact >  m_prollfps;
//      ::user::plain_edit_impact *                        m_prollfps;
      //::user::interaction *                        m_prollspf;

      pane_impact();
      ~pane_impact() override;


      void on_create_impact(::user::impact_data * pimpactdata) override;

      void install_message_routing(::channel * pchannel) override;

      // void assert_ok() const override;

      // void dump(dump_context & dumpcontext) const override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;
      void on_change_cur_sel() override;


      void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;


   };


} // namespace app_core_vulken













