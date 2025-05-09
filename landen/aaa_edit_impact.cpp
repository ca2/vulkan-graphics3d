#include "framework.h"
#include "edit_impact.h"
#include "impact.h"
#include "main_impact.h"
#include "toggle_impact.h"
#include "top_impact.h"
#include "aura/message/user.h"


namespace app_core_vulken
{


   edit_impact::edit_impact()
   {

      m_ptopimpact = nullptr;

   }

   edit_impact::~edit_impact()
   {

   }
   void edit_impact::install_message_routing(::channel * pchannel)
   {

      ::user::show < ::user::plain_edit >::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create,pchannel,this,&edit_impact::on_message_create);

   }


   void edit_impact::on_message_create(::message::message * pmessage)
   {
      ::pointer<::message::create>pcreate(pmessage);

      pcreate->previous();

      if(pcreate->m_bRet)
         return;


      m_ptopimpact = get_typed_parent < top_impact >();
   }


   void edit_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

   }


   void edit_impact::_001OnAfterChangeText(const ::action_context & context)
   {

      if(context.is(::e_source_sync))
         return;

      if(m_ptopimpact == nullptr)
         return;

      impact * pimpact = dynamic_cast < ::app_core_vulken::impact * > (m_ptopimpact->get_typed_parent<::user::split_impact>()->get_child_by_id("vulken_impact"_ansi));

      if(pimpact != nullptr)
      {


         string strText;

         _001GetText(strText);


      }

   }


   /*bool edit_impact::keyboard_focus_is_focusable()
   {

      return is_window_enabled() && is_window_visible();

   }

   bool edit_impact::is_window_visible()
   {

      return ::user::show < ::user::plain_edit >::is_window_visible() && !get_top_level()->frame_is_transparent();

   }


   long long edit_impact::increment_reference_count()
   {
      return ::object::increment_reference_count();
   }
   long long edit_impact::decrement_reference_count()
   {
      return ::object::decrement_reference_count();
   }


   */
} // namespace app_core_vulken




