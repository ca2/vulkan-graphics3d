#include "framework.h"
#include "main_frame.h"
#include "acme/constant/message.h"


namespace lowland_continuum
{


   main_frame::main_frame()
   {

      window_enable_full_screen();

      m_etranslucencyFrame = ::user::e_translucency_present;

   }


   main_frame::~main_frame()
   {

   }


   ::pointer < ::experience::frame > main_frame::frame_experience()
   {

      auto pframe = lowland_landen::frame::frame_experience(nullptr, "022", "LightBlue");

      return pframe;

   }


//   bool main_frame::has_pending_graphical_update()
//   {
//
//      return ::simple_main_frame::has_pending_graphical_update();
//
//   }


   void main_frame::install_message_routing(::channel * pchannel)
   {

      lowland_landen::frame::install_message_routing(pchannel);

      MESSAGE_LINK(e_message_create, pchannel, this, &lowland_landen::frame::on_message_create);

   }


   void main_frame::on_message_create(::message::message * pmessage)
   {

      //set_bitmap_source("Simple papp!!");

   }


} // namespace lowland_continuum



