// From ./windows by camilo on 2021-03-02 17:22 <3ThomasBorregaardSorensen!!
// Created by camilo on 2022-01-29 14:18 <3ThomasBorregaardSorensen!!
#pragma once



#include "acme/operating_system.h"


namespace macos
{


	class CLASS_DECL_APP_CORE_HELLO_MULTIVERSE animated_wallpaper :
		virtual public ::animated_wallpaper
	{
	public:

//		static bool							s_bClassRegistered;
//		static WCHAR						s_wszTitle[MAX_LOADSTRING];
//		static WCHAR						s_wszWindowClass[MAX_LOADSTRING];
//
//		HINSTANCE							m_hinstance;
//		HWND									m_hwnd;

		int_rectangle   					m_rectParentClient;

		animated_wallpaper();
		~animated_wallpaper() override;


		void open() override;
		void close() override;


//      static ATOM _s_register_class(HINSTANCE hInstance);
//  
//		virtual int _update_window_styles(UINT styleFilter, UINT styleExFilter, UINT styleAdd, UINT styleExAdd);
//
//		virtual HWND _get_wallpaper_window();
//		virtual void _set_window();
//
//		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
//
//		virtual LRESULT _wnd_proc(UINT message, WPARAM wParam, LPARAM lParam);


		void update_wallpaper() override;

	};



} // namespace macos
