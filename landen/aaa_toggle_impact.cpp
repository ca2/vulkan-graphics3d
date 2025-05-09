#include "framework.h"
#include "toggle_impact.h"


namespace app_core_vulken
{


   toggle_impact::toggle_impact()
   {

      m_ptopimpact = nullptr;

   }



   toggle_impact::~toggle_impact()
   {


   }

   void toggle_impact::install_message_routing(::channel * pchannel)
   {

      ::user::impact::install_message_routing(pchannel);
      ::user::button::install_message_routing(pchannel);

   }


   void toggle_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      ::user::impact::handle(ptopic, phandlercontext);

   }


   bool toggle_impact::keyboard_focus_is_focusable()
   {

      return ::user::button::keyboard_focus_is_focusable() ;

   }


} // namespace app_core_vulken













