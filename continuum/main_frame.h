#pragma once


#include "vulkan-land/landen/frame.h"


namespace vulkan_land_continuum
{


   class CLASS_DECL_VULKAN_LAND_CONTINUUM main_frame :
      virtual public vulkan_land_landen::frame
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


} // namespace vulkan_land_continuum



