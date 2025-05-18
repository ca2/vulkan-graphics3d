// Created by camilo on 2023-04-27 17:06 <3ThomasBorregaardSorensen!!
// From draw2d_vulkan by camilo on 2025-05-16 07:41 <3ThomasBorregaardSorensen!!
#pragma once


#undef USUAL_OPERATING_SYSTEM_SUPPRESSIONS
#include "acme/_operating_system.h"

#if defined(WINDOWS_DESKTOP)
#define VK_PROTOTYPES
#define VK_USE_PLATFORM_WIN32_KHR
#else
#error "wwwwwhat?!?!"
#endif


#include <vulkan/vulkan.h>



