#pragma once


#include "core/_.h"



#if defined(_lowland_vulkan_project)
   #define CLASS_DECL_GRAPHICS3D_VULKAN  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_GRAPHICS3D_VULKAN  CLASS_DECL_IMPORT
#endif


namespace vulkan_land_landen
{


   class application;
   //class top_impact;
   //class main_impact;
   class document;
   class impact;


} // namespace vulkan_land_landen


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"


#if defined(WINDOWS_DESKTOP)

#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR

#endif



