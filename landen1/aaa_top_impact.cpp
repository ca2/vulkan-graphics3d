#include "framework.h"
#include "edit_impact.h"
#include "toggle_impact.h"
#include "top_impact.h"


namespace app_core_vulken
{


   top_impact::top_impact()
   {

      m_peditimpact = nullptr;
      m_ptoggleimpact = nullptr;

   }


   top_impact::~top_impact()
   {

   }


   //void top_impact::assert_ok() const
   //{

   //   ::user::split_impact::assert_ok();

   //}


   //void top_impact::dump(dump_context & dumpcontext) const
   //{

   //   ::user::split_impact::dump(dumpcontext);

   //}


   void top_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {
      ::user::split_impact::handle(ptopic, phandlercontext);
   }


   void top_impact::on_create_split_impact()
   {

      if(get_pane_count() > 0)
         return;

      SetPaneCount(2);

      SetSplitOrientation(e_orientation_vertical);

      set_position_rate(0,0.84);

      initialize_split_layout();

      m_peditimpact = create_impact < edit_impact >(nullptr, get_pane_holder(0),"top_edit_impact"_ansi);

      if(m_peditimpact == nullptr)
      {

         output_error_message("Could not create folder edit impact"_ansi);

      }

      m_peditimpact->m_ptopimpact = this;

      m_ptoggleimpact = create_impact < toggle_impact >(nullptr, get_pane_holder(1),"top_toggle_impact"_ansi);

      if(m_ptoggleimpact == nullptr)
      {

         output_error_message("Could not create file list ::user::impact"_ansi);

      }

      m_ptoggleimpact->m_ptopimpact = this;



   }


} // namespace app_core_vulken



