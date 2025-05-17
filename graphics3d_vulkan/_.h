#pragma once


#include "core/_.h"
#include "_vulkan.h"


#if defined(_graphics3d_vulkan_project)
   #define CLASS_DECL_GRAPHICS3D_VULKAN  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_GRAPHICS3D_VULKAN  CLASS_DECL_IMPORT
#endif


namespace graphics3d_vulkan
{


   class application;
   //class top_impact;
   //class main_impact;
   class document;
   class impact;


} // namespace graphics3d_vulkan


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"


#if defined(WINDOWS_DESKTOP)

#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#endif



