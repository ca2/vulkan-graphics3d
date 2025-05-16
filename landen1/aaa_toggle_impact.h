#pragma once


#include "base/user/user/show.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN toggle_impact:
      virtual public ::user::show < ::user::button >
   {
   public:


      top_impact *           m_ptopimpact;


      toggle_impact();
      virtual ~toggle_impact();


      void install_message_routing(::channel * pchannel) override;


      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;


      virtual bool keyboard_focus_is_focusable();


   };


} // namespace app_core_vulken




