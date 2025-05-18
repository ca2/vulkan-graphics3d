#pragma once


#include "app-cube/cube/_.h"
#include "_vulkan.h"


#if defined(_graphics3d_vulkan_project)
   #define CLASS_DECL_GRAPHICS3D_VULKAN  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_GRAPHICS3D_VULKAN  CLASS_DECL_IMPORT
#endif


namespace graphics3d_vulkan
{


   class application_handler;

   class renderer;
   class application_object;


   class model;
   class context;
   class input;
   //class camera;
   class pipeline;

   class descriptor_pool;

} // namespace graphics3d_vulkan


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"




