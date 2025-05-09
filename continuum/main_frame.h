#pragma once


#include "lowland/landen/frame.h"


namespace lowland_continuum
{


   class CLASS_DECL_lowland_continuum main_frame :
      virtual public lowland_landen::frame
   {
   public:


      bool                       m_bExplicitTranslucency;


      main_frame();
      ~main_frame() override;


//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;


      ::pointer < ::experience::frame > frame_experience() override;


      //bool has_pending_graphical_update() override;


      void install_message_routing(::channel * pchannel) override;


      DECLARE_MESSAGE_HANDLER(on_message_create);


   };


} // namespace lowland_continuum



