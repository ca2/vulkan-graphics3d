#pragma once


#include "base/user/user/split_impact.h"


namespace app_core_vulken
{


   class CLASS_DECL_APP_CORE_VULKEN top_impact:
      virtual public ::user::split_impact
   {
   public:


      edit_impact *                   m_peditimpact;
      toggle_impact *                 m_ptoggleimpact;


      top_impact();
      virtual ~top_impact();


      // void assert_ok() const override;

      // void dump(dump_context & dumpcontext) const override;

      void on_create_split_impact();

      void handle(::topic * ptopic, ::handler_context * phandlercontext) override;

      //virtual bool is_window_visible() override;


   };


} // namespace app_core_vulken







