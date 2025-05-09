#include "framework.h"
#include "pane_impact.h"
#include "acme/constant/id.h"
#include "aura/user/user/plain_edit.h"
#include "apex/database/stream.h"
#include "base/user/form/document.h"
#include "base/user/user/show.h"
#include "base/user/user/single_document_template.h"
#include "base/user/user/tab_pane.h"
#include "core/platform/session.h"
#include "core/user/user/user.h"


namespace app_core_vulken
{


   pane_impact::pane_impact()
   {

      //m_pimpactLast = nullptr;
      //m_prollfps = nullptr;

   }


   pane_impact::~pane_impact()
   {

   }


   //void pane_impact::assert_ok() const
   //{

   //   ::user::impact::assert_ok();

   //}


   //void pane_impact::dump(dump_context & dumpcontext) const
   //{

   //   ::user::impact::dump(dumpcontext);

   //}


   void pane_impact::install_message_routing(::channel * pchannel)
   {

      ::userex::pane_tab_impact::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create, pchannel, this, &pane_impact::on_message_create);

   }


   void pane_impact::on_message_create(::message::message * pmessage)
   {
      if(pmessage->previous())
         return;

      set_tab("Menu"_ansi,MENU_IMPACT);
      set_tab("vulken"_ansi, MAIN_IMPACT);
      set_tab("Font"_ansi,FONTSEL_IMPACT);
      set_tab("Open"_ansi, FILEMANAGER_IMPACT);

      set_current_tab_by_id(MAIN_IMPACT);

   }

   void pane_impact::on_change_cur_sel()
   {
      ::userex::pane_tab_impact::on_change_cur_sel();
//      string strId = get_impact_id();
//      string_array stra;
//      stra.explode("->:<-"_ansi,strId);
//      if(get_impact_id() == MAIN_IMPACT
//            || get_impact_id() == MAIN_SWITCHER_IMPACT
//            || stra.contains(::as_string((int)MAIN_IMPACT))
//            || stra.contains(::as_string((int)MAIN_SWITCHER_IMPACT)))
//      {
//
//         auto ptabpaneMenu = get_tab_by_id(MENU_IMPACT);
//
//         if(ptabpaneMenu->m_pplaceholder != nullptr)
//         {
//
//            ptabpaneMenu->m_pplaceholder->hide();
//
//         }
//
//         auto ptabpaneFileManager = get_tab_by_id(FILEMANAGER_IMPACT);
//
//         if(ptabpaneFileManager != nullptr && ptabpaneFileManager->m_pplaceholder != nullptr)
//         {
//
//            ptabpaneFileManager->m_pplaceholder->hide();
//
//         }
//
//         //if(ptabpaneFontSel->m_pplaceholder != nullptr)
//         //{
//
//         //   ptabpaneFontSel->m_pplaceholder->hide();
//
//         //}
//
//         if(get_impact_id() == MAIN_IMPACT)
//         {
//
//            m_pimpactLast = dynamic_cast < impact * > (tab_holder(id_index(MAIN_IMPACT))->get_child_by_id("vulken_impact"_ansi));
//
//         }
//         else if (get_impact_id() == MAIN_SWITCHER_IMPACT)
//         {
//
//            m_pimpactLast = dynamic_cast < impact * > (tab_holder(id_index(MAIN_SWITCHER_IMPACT))->get_child_by_id("vulken_impact"_ansi));
//
//         }
//
//
//      }
//      else if(get_impact_id() == MENU_IMPACT)
//      {
//
//         //string str;
//
//         //auto papp = m_papp;
//
//         //papp->datastream()->get(m_pimpactLast->id()+".cur_fps_text"_ansi,str);
//
//         //m_prollfps->_001SetText(str, ::e_source_database);
//
//      }
////      else if(get_impact_id() == FONTSEL_IMPACT)
////      {
////
////         ::pointer<FONTSEL_IMPACT>>m_pholder->get_child_by_id("font_list_impact"_ansi);
////
////         if(m_pimpactLast != nullptr && pfontview.is_set())
////         {
////
////            pfontview->set_sel_by_name(m_pimpactLast->m_strFont);
////
////         }
////
////      }

   }


   void pane_impact::on_create_impact(::user::impact_data * pimpactdata)
   {

      switch(pimpactdata->id().as_int())
      {
      case CONTEXT_MENU_IMPACT:
      {

//         m_pdocMenu = session()->user()->create_child_form(this, nullptr, this, pimpactdata->m_pplaceholder);
//
//         m_pdocMenu->open_document("matter://menu.html"_ansi);
//
//         ::pointer<::user::impact>pimpact = m_pdocMenu->get_impact();
//
//         m_prollfps = pimpact->get_child_by_id("roll_per_second"_ansi);
////         m_prollspf = pimpact->get_child_by_id("roll_spf"_ansi);
//         //try
//         //{
//         //   pform->get_child_by_id("clockverse_auto"_ansi)->_001SetCheck(papp->should_auto_launch_clockverse_on_hover(),::e_source_initialize);
//         //}
//         //catch(...)
//         //{
//         //}
//
//         //try
//         //{
//         //   pform->get_child_by_id("flag"_ansi)->_001SetCheck(papp->should_bind_flag_country_ca2_domain_image_on_hover(),::e_source_initialize);
//         //}
//         //catch(...)
//         //{
//         //}
//
//         //try
//         //{
//         //   pform->get_child_by_id("flag_auto"_ansi)->_001SetCheck(papp->should_auto_launch_flag_on_hover(),::e_source_initialize);
//         //}
//         //catch(...)
//         //{
//         //}
//


      }
      break;
      case MAIN_IMPACT:
      {
         ::pointer<::user::document>pdocument =  m_papp->impact_system("impact"_ansi)->open_document_file(get_app(), nullptr,true,pimpactdata->m_pplaceholder);


      }

      break;



      }

      ::userex::pane_tab_impact::on_create_impact(pimpactdata);

   }


   void pane_impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      //if(m_pdocMenu != nullptr && dynamic_cast < ::user::impact * > (ptopic->user_interaction()) == m_pdocMenu->get_impact(0))
      //{

      //   if(ptopic->id() == ::id_after_change_text && ptopic->user_interaction() != nullptr)
      //   {

      //      if(m_prollfps != nullptr && ptopic->user_interaction_id() == "roll_per_second"_ansi && !ptopic->m_actioncontext.is(::e_source_initialize)
      //            && !ptopic->m_actioncontext.is(::e_source_database))
      //      {


      //      }
      //      //else if(ptopic->user_interaction_id() == "roll_spf"_ansi && !ptopic->m_context.is_source(::e_source_initialize))
      //      //{

      //      //   try
      //      //   {
      //      //      string str;
      //      //      m_prollspf->_001GetText(str);
      //      //      double d = atof(str);
      //      //      if(fabs(d) < 0.0000000001)
      //      //      {
      //      //         m_prollfps->_001SetText(""_ansi,::e_source_initialize);
      //      //      }
      //      //      else
      //      //      {
      //      //         m_pflagview->m_frequencyFramesPerSecond = 1.0/ d;
      //      //         m_pflagview->m_timeRoll= ::time::now();
      //      //         m_prollfps->_001SetText(as_string(1.0 / d),::e_source_initialize);
      //      //      }
      //      //   }
      //      //   catch(...)
      //      //   {
      //      //   }

      //      //}


      //   }

      //   //   if(ptopic->user_interaction_id() == "clockverse"_ansi)
      //   //   {
      //   //      papp->set_binding_clockverse_country_time_zone_set_on_hover(ptopic->user_interaction()->_001GetCheck() == ::e_check_checked);
      //   //      return true;
      //   //   }
      //   //   else if(ptopic->user_interaction_id() == "clockverse_auto"_ansi)
      //   //   {
      //   //      papp->set_auto_launch_clockverse_on_hover(ptopic->user_interaction()->_001GetCheck() == ::e_check_checked);
      //   //      return true;
      //   //   }
      //   //   else if(ptopic->user_interaction_id() == "flag"_ansi)
      //   //   {
      //   //      papp->set_binding_flag_country_ca2_domain_image_on_hover(ptopic->user_interaction()->_001GetCheck() == ::e_check_checked);
      //   //      return true;
      //   //   }
      //   //   else if(ptopic->user_interaction_id() == "flag_auto"_ansi)
      //   //   {
      //   //      papp->set_auto_launch_flag_on_hover(ptopic->user_interaction()->_001GetCheck() == ::e_check_checked);
      //   //      return true;
      //   //   }
      //   //}
      //}

      ////return false;
   }



   void pane_impact::_001OnNcDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::userex::pane_tab_impact::_001OnNcDraw(pgraphics);

   }


   void pane_impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      ::userex::pane_tab_impact::_001OnDraw(pgraphics);

   }




} // namespace app_core_vulken



