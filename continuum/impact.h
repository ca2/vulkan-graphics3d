#pragma once


#include "vulkan-land/landen/impact.h"


namespace vulkan_land_continuum
{


   class CLASS_DECL_vulkan_land_continuum impact :
      virtual public ::app_consumer < application, ::vulkan_land_landen::impact >
   {
   public:


      ::write_text::font_pointer       m_pfontThomasBS_;
      string                           m_strFont1;
      int                              m_iSequence;


      impact();
      ~impact() override;

//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif

      ::pointer < ::vulkan::application > start_vulkan_application() override;


      virtual void install_message_routing(::channel * psender) override;

      virtual void _001OnClip(::draw2d::graphics_pointer & pgraphics) override;

      virtual void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      document * get_document();

      void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      
      
      bool on_click(::item * pitem, ::user::mouse * pmouse) override;
      
      bool is_absolute_mouse_position() override;

   };


} // namespace vulkan_land_continuum


