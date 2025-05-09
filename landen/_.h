#pragma once


#include "core/_.h"



#if defined(_lowland_landen_project)
   #define CLASS_DECL_LOWLAND_LANDEN  CLASS_DECL_EXPORT
#else
   #define CLASS_DECL_LOWLAND_LANDEN  CLASS_DECL_IMPORT
#endif


namespace lowland_landen
{


   class application;
   //class top_impact;
   //class main_impact;
   class document;
   class impact;


} // namespace lowland_landen


//#undef APP_TYPE
//#define APP_TYPE ::vulken::application


//#include "application.h"




