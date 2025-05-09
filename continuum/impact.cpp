#include "framework.h"
#include "impact.h"
#include "document.h"
#include "application.h"
#include "acme/constant/message.h"
#include "acme/filesystem/filesystem/file_system.h"
#include "acme/filesystem/filesystem/file_context.h"
#include "acme/platform/node.h"
#include "acme/prototype/mathematics/mathematics.h"
#include "aura/message/user.h"
#include "aura/graphics/draw2d/draw2d.h"
#include "apex/database/stream.h"


CLASS_DECL_AURA ::color::color dk_red(); // <3 tbs
namespace vkc
{
   ::pointer<::vulkan::application > start_vulkan_application(::vkc::VkContainer* pvkcontainer, mouseState* pmousestate);
} // namespace vkc

namespace lowland_continuum
{




   impact::impact()
   {

      m_flagNonClient -= e_non_client_background;
      m_iSequence = 0;
      m_bNeedFullRedrawOnResize = true;
      
      m_bDefaultClickHandling = true;
      
      m_bEmptyAreaIsClientArea = true;

   }


   impact::~impact()
   {

   }


#ifdef _DEBUG


   long long impact::increment_reference_count()
   {

      return ::user::impact::increment_reference_count();

   }


   long long impact::decrement_reference_count()
   {

      return ::user::impact::decrement_reference_count();

   }


#endif



   void impact::install_message_routing(::channel * psender)
   {

      ::lowland_landen::impact::install_message_routing(psender);

      MESSAGE_LINK(e_message_create,psender,this,&impact::on_message_create);
      MESSAGE_LINK(e_message_destroy, psender, this, &impact::on_message_destroy);

   }



   ::pointer < ::vulkan::application > impact::start_vulkan_application()
   {

      return ::vkc::start_vulkan_application(this, &m_mousestate);

   }



   void impact::on_message_create(::message::message * pmessage)
   {

      ::pointer<::message::create>pcreate(pmessage);

      //display();

      pcreate->previous();

      if (pcreate->m_bRet)
      {

         return;

      }


      //application()->show_about_box();
    

   }


   void impact::on_message_destroy(::message::message * pmessage)
   {

   }




   void impact::handle(::topic * ptopic, ::handler_context * phandlercontext)
   {

      ::user::impact::handle(ptopic, phandlercontext);

   }


   document * impact::get_document()
   {

      return dynamic_cast < document * >(::user::impact::get_document());

   }


   void impact::_001OnClip(::draw2d::graphics_pointer & pgraphics)
   {


   }


   void impact::_001OnDraw(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if (rectangleX.is_empty())
      {

         return;

      }

//      if (pgraphics->payload("set_transparent") == "set_transparent")
//      {
//
//         information() << "set_transparent called";
//
//      }
//      else
//      {
//
//         information() << "set_transparent NOT called!!";
//
//      }

      ::double_rectangle rectangleClipBox;

      //pgraphics->reset_clip();

      //pgraphics->get_clip_box(rectangleClipBox);

      auto matrix = pgraphics->m_matrix;

      //auto origin = pgraphics->origin();

      auto opacity = ::opacity(48);

      pgraphics->set_alpha_mode(::draw2d::e_alpha_mode_blend);

      pgraphics->fill_rectangle(rectangleX, argb(108, 128, 128, 128));

      ::color::color color_dk(dk_red());

#ifdef DEBUG_WORK

      ::int_rectangle rectangleDryProWithLove_Work(5, 5, 1915, 1075);

      pgraphics->fill_rectangle(rectangleDryProWithLove_Work, argb(255, 150, 200, 255));

#endif

      pgraphics->set_text_color(color_dk);

      auto psystem = system();

      auto pnode = psystem->draw2d();

      auto pwritetext = pnode->write_text();

      auto fontsize = ::write_text::font_size(48.0 * rectangleX.height() / 1'080, e_unit_pixel);

      m_pfontThomasBS_ = pwritetext->font("Fira Code", fontsize);

      pgraphics->set(m_pfontThomasBS_);

      pgraphics->set_text_rendering_hint(write_text::e_rendering_anti_alias);

      pgraphics->set_alpha_mode(draw2d::e_alpha_mode_blend);

      _synchronous_lock synchronouslockDocument(get_document()->synchronization());
      
      string_array & stra = get_document()->m_straLine;
      
      bool bWhite  = true;
      
      double x = 0.;
      
      double y = 0.;
      
      ::int_point point;

      string strText;

      m_iSequence++;
      
      strText.formatf("psimpleapplication->m_pimpact->_001OnDraw(pgraphics); %d", m_iSequence);

      auto size = pgraphics->get_text_extent(strText);

      bool bFixedPosition = true;

      if (bFixedPosition)
      {

         point = { 10, 10 };

      }
      else
      {
       
         point.x() = mathematics()->random(0, (int)(rectangleX.width() - size.cx()));
         point.y() = mathematics()->random(0, (int)(rectangleX.height() - size.cy() * 2));

      }

      ::color::color color;

      opacity = ::opacity(96);

      for(auto & strItem : stra)
      {
         
         if(strItem.is_empty())
         {
            
            x = 0;
            
            y += size.cy();
            
            bWhite = true;
         
            continue;
         
         }

         if(bWhite)
         {
            
            color = color::white;
            
         }
         else
         {
            
            color = color_dk;
            
         }

         color &= opacity;

         pgraphics->set_text_color(color);

         pgraphics->text_out(point.x() + x, point.y() + y, strItem);
         
         auto s = pgraphics->get_text_extent(strItem);

         x += s.cx();
         
         bWhite = !bWhite;
         
      }

      color = color_dk;
      
      color &= opacity;

      pgraphics->set_text_color(color);

      pgraphics->text_out(point.x(), point.y() + y + size.cy(), strText);

      pgraphics->set_smooth_mode(::draw2d::e_smooth_mode_none);
      ::lowland_landen::impact::_001OnDraw(pgraphics);
   }


   void impact::on_layout(::draw2d::graphics_pointer & pgraphics)
   {

      auto rectangleX = this->rectangle();

      if(rectangleX.is_empty())
      {

         return;

      }

      ::lowland_landen::impact::on_layout(pgraphics);
      
      setup_default_client_area_user_item();

   }


   bool impact::on_click(::item * pitem, ::user::mouse * pmouse)
   {

      if (::is_set(pitem))
      {

         if (pitem->m_item.m_eelement == ::e_element_client)
         {

            print_line("on_click : e_element_client");
            
            ::array < ::file::file_dialog_filter > filtera;
            
            filtera.add({"application.txt", "application.txt"});
            
            pick_single_file(filtera, [ this ] (const ::file::path & path)
                             {
               
               try {
                  auto memory = file()->as_memory(path);
                  
                  auto size = memory.size();
                  
                  informationf("got file with %d bytes", size);
                  
                  file()->put_memory(m_papp->m_pathApplicationText, memory);
                  

               } catch (...) {
                  auto pmessagebox = message_box("No file loaded...");
                  pmessagebox->async();
               }
               
            }, false);

            return true;

         }

      }

      return ::user::impact::on_click(pitem, pmouse);

   }


   bool impact::is_absolute_mouse_position()
   {


      bool bAbsoluteMousePosition = false;

      get_app()->datastream()->get("Absolute Mouse Position", bAbsoluteMousePosition);

      return bAbsoluteMousePosition;

   }

} // namespace lowland_continuum



