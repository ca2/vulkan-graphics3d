#include "framework.h"
#include "impact.h"
#include "main_impact.h"


namespace app_core_vulken
{


   main_impact::main_impact()
   {

      m_ptopimpact = nullptr;
      m_pimpact    = nullptr;

   }


   main_impact::~main_impact()
   {

   }


   //void main_impact::assert_ok() const
   //{

   //   ::user::split_impact::assert_ok();

   //}


   //void main_impact::dump(dump_context & dumpcontext) const
   //{

   //   ::user::split_impact::dump(dumpcontext);

   //}


   void main_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {
      ::user::split_impact::handle(ptopic, phandlercontext);
   }


   void main_impact::on_create_split_impact()
   {

      if(get_pane_count() > 0)
         return;

      SetPaneCount(1);

      SetSplitOrientation(e_orientation_horizontal);

      initialize_split_layout();

      m_pimpact = create_pane_impact < ::app_core_vulken::impact >(0, "vulken_impact"_ansi);

      if(m_pimpact == nullptr)
      {

         output_error_message("Could not create file list ::user::impact"_ansi);

      }

   }

   
   void main_impact::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
   {


   }


   void main_impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {


   }


   //void main_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   //{

   //   if(ptopic->id() == ::id_click)
   //   {

   //      if(ptopic->user_interaction_id() == "top_toggle_impact"_ansi)
   //      {

   //      }

   //   }

   //}


} // namespace app_core_vulken



