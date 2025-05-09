#pragma once


#include "aura/user/user/plain_edit.h"
#include "base/user/user/show.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN edit_impact:
      virtual public ::user::show < ::user::plain_edit >
   {
   public:


      top_impact * m_ptopimpact;


      edit_impact();
      virtual ~edit_impact();

      virtual void _001OnAfterChangeText(const ::action_context & action_context);

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      void install_message_routing(::channel * pchannel) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      //bool keyboard_focus_is_focusable();

      //virtual long long increment_reference_count();

      //virtual long long decrement_reference_count();

      //bool is_window_visible() override;

   };


} // namespace app_core_vulken




