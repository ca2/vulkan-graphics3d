#pragma once


#include "app-cube/cube/document.h"


namespace vulkan_graphics3d_continuum
{


   class CLASS_DECL_VULKAN_GRAPHICS3D_CONTINUUM document :
      public ::cube::document
   {
   public:
      
      
      string_array   m_straLine;


      document();
	   ~document() override;


	   // void assert_ok() const override;
	   // void dump(dump_context & dumpcontext) const override;

      bool on_new_document() override;
      virtual bool on_open_document(const ::payload & payloadFile) override;

#ifdef _DEBUG
      virtual long long increment_reference_count() override;
      virtual long long decrement_reference_count() override;
#endif

      //virtual ::stream& write(::stream& stream) const override;


   };


} // namespace vulkan_graphics3d_continuum



