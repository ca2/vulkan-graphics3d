#pragma once


#include "base/user/simple/frame_window.h"


namespace vulkan_graphics3d_continuum
{


   class CLASS_DECL_VULKAN_GRAPHICS3D_CONTINUUM main_frame :
      virtual public ::simple_frame_window
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


} // namespace vulkan_graphics3d_continuum



