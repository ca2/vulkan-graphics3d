#pragma once


//#include <GL/gl.h>
//#include <GL/glu.h>
//#include <GL/glext.h>
//#ifdef LINUX
//#include <GL/glx.h>
//#endif

//-----------------------------------------------------------------------------
// Copyright (c) 2006-2010 dhpoware. All Rights Reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"_ansi),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, topic to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS"_ansi, WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------
//
// This demo builds on the previous layered window demo.
// (http://www.dhpoware.com/downloads/LayeredWindow.zip).
//
// In the previous demo, we used a TGA image with an embedded alpha mask as the
// source of our layered window.
//
// In this demo we use OpenGL to draw a rotating cube to a pbuffer. Once the
// scene is drawn to the pbuffer we then make a local system memory copy of
// the pbuffer's pixel data and use that as the source of our layered window.
//
// You can transfer the cube around the screen by holding down the left mouse
// button and dragging. To exit the demo press the ESC key.
//
// This demo requires Windows 2000, XP, or higher to run. The minimum supported
// operating system for the layered windows API is Windows 2000.
//
// This demo also requires the following OpenGL extensions:
//  WGL_ARB_pbuffer
//  WGL_ARB_pixel_format
//
//-----------------------------------------------------------------------------

// Force the layered windows APIs to be visible.
//#define _WIN32_WINNT 0x0500

// Disable warning C4244: conversion from 'float' to 'unsigned char', possible loss of
// data. Used in the ImagePreMultAlpha() function.
#pragma warning (disable : 4244)

////#include <windows.h>
//#include <GL/gl.h>
//#include <GL/glu.h>
//
////-----------------------------------------------------------------------------
//// WGL_ARB_pbuffer.
////-----------------------------------------------------------------------------
//
//#define WGL_DRAW_TO_PBUFFER_ARB                   0x202D
//#define WGL_MAX_PBUFFER_HEIGHT_ARB                0x2030
//#define WGL_MAX_PBUFFER_PIXELS_ARB                0x202E
//#define WGL_MAX_PBUFFER_WIDTH_ARB                 0x202F
//#define WGL_PBUFFER_HEIGHT_ARB                    0x2035
//#define WGL_PBUFFER_LARGEST_ARB                   0x2033
//#define WGL_PBUFFER_LOST_ARB                      0x2036
//#define WGL_PBUFFER_WIDTH_ARB                     0x2034
//
//DECLARE_HANDLE(HPBUFFERARB);
//
//
////-----------------------------------------------------------------------------
//// WGL_ARB_pixel_format.
////-----------------------------------------------------------------------------
//
//#define WGL_ACCELERATION_ARB                      0x2003
//#define WGL_ACCUM_ALPHA_BITS_ARB                  0x2021
//#define WGL_ACCUM_BITS_ARB                        0x201D
//#define WGL_ACCUM_BLUE_BITS_ARB                   0x2020
//#define WGL_ACCUM_GREEN_BITS_ARB                  0x201F
//#define WGL_ACCUM_RED_BITS_ARB                    0x201E
//#define WGL_ALPHA_BITS_ARB                        0x201B
//#define WGL_ALPHA_SHIFT_ARB                       0x201C
//#define WGL_AUX_BUFFERS_ARB                       0x2024
//#define WGL_BLUE_BITS_ARB                         0x2019
//#define WGL_BLUE_SHIFT_ARB                        0x201A
//#define WGL_COLOR_BITS_ARB                        0x2014
//#define WGL_DEPTH_BITS_ARB                        0x2022
//#define WGL_DOUBLE_BUFFER_ARB                     0x2011
//#define WGL_DRAW_TO_BITMAP_ARB                    0x2002
//#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
//#define WGL_FULL_ACCELERATION_ARB                 0x2027
//#define WGL_GENERIC_ACCELERATION_ARB              0x2026
//#define WGL_GREEN_BITS_ARB                        0x2017
//#define WGL_GREEN_SHIFT_ARB                       0x2018
//#define WGL_NEED_PALETTE_ARB                      0x2004
//#define WGL_NEED_SYSTEM_PALETTE_ARB               0x2005
//#define WGL_NO_ACCELERATION_ARB                   0x2025
//#define WGL_NUMBER_OVERLAYS_ARB                   0x2008
//#define WGL_NUMBER_PIXEL_FORMATS_ARB              0x2000
//#define WGL_NUMBER_UNDERLAYS_ARB                  0x2009
//#define WGL_PIXEL_TYPE_ARB                        0x2013
//#define WGL_RED_BITS_ARB                          0x2015
//#define WGL_RED_SHIFT_ARB                         0x2016
//#define WGL_SHARE_ACCUM_ARB                       0x200E
//#define WGL_SHARE_DEPTH_ARB                       0x200C
//#define WGL_SHARE_STENCIL_ARB                     0x200D
//#define WGL_STENCIL_BITS_ARB                      0x2023
//#define WGL_STEREO_ARB                            0x2012
//#define WGL_SUPPORT_GDI_ARB                       0x200F
//#define WGL_SUPPORT_OPENGL_ARB                    0x2010
//#define WGL_SWAP_COPY_ARB                         0x2029
//#define WGL_SWAP_EXCHANGE_ARB                     0x2028
//#define WGL_SWAP_LAYER_BUFFERS_ARB                0x2006
//#define WGL_SWAP_METHOD_ARB                       0x2007
//#define WGL_SWAP_UNDEFINED_ARB                    0x202A
//#define WGL_TRANSPARENT_ALPHA_VALUE_ARB           0x203A
//#define WGL_TRANSPARENT_ARB                       0x200A
//#define WGL_TRANSPARENT_BLUE_VALUE_ARB            0x2039
//#define WGL_TRANSPARENT_GREEN_VALUE_ARB           0x2038
//#define WGL_TRANSPARENT_INDEX_VALUE_ARB           0x203B
//#define WGL_TRANSPARENT_RED_VALUE_ARB             0x2037
//#define WGL_TYPE_COLORINDEX_ARB                   0x202C
//#define WGL_TYPE_RGBA_ARB                         0x202B
//
//
////-----------------------------------------------------------------------------
//// Globals.
////-----------------------------------------------------------------------------

#define TEXTURE_WIDTH   64
#define TEXTURE_HEIGHT  64


#include "base/user/user/impact.h"
#include "vulkan/shared.h"
#include "vulkan/application.h"
#include "vulkan/vk_container.h"
#include "apex/platform/app_consumer.h"
#include "vulkan-land/landen/vulkan/vk_container.h"



namespace vulkan_land_landen
{


   class CLASS_DECL_VULKAN_LAND_LANDEN impact :
      virtual public ::app_consumer < application, ::user::impact, ::vkc::VkContainer>
   {
   public:

      //memory m_memory;
      //int m_i;

      mouse_state                   m_mousestate;
      ::image::image_pointer        m_pimage;

      ::particle_pointer            m_pparticleImageSynchronization;



      impact();
      ~impact() override;

//      // void assert_ok() const override;
//      // void dump(dump_context & dumpcontext) const override;
      ::int_size size() override;



      void install_message_routing(::channel * pchannel) override;

      ::user::enum_key_state get_key_state(::user::e_key ekey) override;
      void _001OnNcClip(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnClip(::draw2d::graphics_pointer & pgraphics) override;


      void _001OnNcDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnDraw(::draw2d::graphics_pointer & pgraphics) override;
      void _001OnNcPostDraw(::draw2d::graphics_pointer & pgraphics) override;


      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      ::user::document * get_document();


      virtual ::pointer < ::vulkan::application > start_vulkan_application();


      void on_layout(::draw2d::graphics_pointer & pgraphics) override;

      DECLARE_MESSAGE_HANDLER(on_message_create);
      DECLARE_MESSAGE_HANDLER(on_message_destroy);
      DECLARE_MESSAGE_HANDLER(on_message_key_down);
      DECLARE_MESSAGE_HANDLER(on_message_key_up);
      DECLARE_MESSAGE_HANDLER(on_message_mouse_move);
      DECLARE_MESSAGE_HANDLER(on_message_left_button_down);
      DECLARE_MESSAGE_HANDLER(on_message_left_button_up);
      DECLARE_MESSAGE_HANDLER(on_message_mouse_leave);

      void on_timer(::timer * ptimer) override;

      void reset_mouse_last_position();


      long long increment_reference_count() override
      {
         return ::object::increment_reference_count();
      }
      long long decrement_reference_count() override
      {
         return ::object::decrement_reference_count();
      }
      bool keyboard_focus_is_focusable() override;

   };


} // namespace vulkan_land_landen



