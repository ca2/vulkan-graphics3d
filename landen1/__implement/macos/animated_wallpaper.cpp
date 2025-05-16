// From ./windows on 2022-03-02 17:13 <3ThomasBorregaardSorensen!!
// Created by camilo on 2022-01-29 14:17 <3ThomasBorregaardSorensen!!
#include "framework.h"

#include "animated_wallpaper.h"
//#include <gdiplus.h>



namespace macos
{


//   bool	animated_wallpaper::s_bClassRegistered = false;
//   WCHAR	animated_wallpaper::s_wszTitle[MAX_LOADSTRING];
//   WCHAR	animated_wallpaper::s_wszWindowClass[MAX_LOADSTRING];

   animated_wallpaper::animated_wallpaper()
   {

      defer_create_synchronization();

      //m_hwnd = nullptr;


   }


   animated_wallpaper::~animated_wallpaper()
   {

   close();

   }

   
//   LRESULT animated_wallpaper::_wnd_proc(UINT message, WPARAM wparam, LPARAM lparam)
//   {
//
//      switch (message)
//      {
//      case WM_COMMAND:
//      {
//         //int wmId = LOWORD(wparam);
//         //// Parse the menu selections:
//         //switch (wmId)
//         //{
//         //case IDM_ABOUT:
//         //   DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
//         //   break;
//         //case IDM_EXIT:
//         //   DestroyWindow(hWnd);
//         //   break;
//         //default:
//         //   return DefWindowProc(hWnd, message, wparam, lparam);
//         //}
//         return DefWindowProc(m_hwnd, message, wparam, lparam);
//      }
//      break;
//      case WM_PAINT:
//      {
//         PAINTSTRUCT ps;
//         HDC hdc = BeginPaint(m_hwnd, &ps);
//
//         synchronous_lock lock(synchronization());
//
//         if(m_pimage.ok())
//         {
//
//         Gdiplus::Graphics g(hdc);
//
//         Gdiplus::Rect r;
//         r.X = 0;
//         r.Y = 0;
//         r.Width = m_cx;
//         r.Height = m_cy;
//
//         m_pimage->map();
//
//         Gdiplus::Bitmap bmp(m_pimage->width(),
//         m_pimage->height(), m_pimage->scan_size(), PixelFormat32bppARGB, (BYTE *)m_pimage->get_data());
//
//         g.DrawImage(&bmp, r);
//
//         }
//
//
//         EndPaint(m_hwnd, &ps);
//      }
//      break;
//      case WM_CLOSE:
//      ::DestroyWindow(m_hwnd);
//      break;
//      case WM_DESTROY:
//         PostQuitMessage(0);
//         break;
//      default:
//         return DefWindowProc(m_hwnd, message, wparam, lparam);
//      }
//      return 0;
//
//   }


//   LRESULT CALLBACK animated_wallpaper::WndProc(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
//   {
//
//      animated_wallpaper * panimatedwallpaper = nullptr;
//
//      if (message == WM_NCCREATE)
//      {
//
//      auto pcreatestruct = (LPCREATESTRUCT) lparam;
//      panimatedwallpaper = (animated_wallpaper *)pcreatestruct->lpCreateParams;
//      panimatedwallpaper->m_hwnd = hWnd;
//      SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)panimatedwallpaper);
//
//      }
//      else
//      {
//
//         panimatedwallpaper = (animated_wallpaper *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
//      }
//
//      if(!panimatedwallpaper)
//      {
//
//         return DefWindowProc(hWnd, message, wparam, lparam);
//
//      }
//
//
//      return panimatedwallpaper->_wnd_proc(message, wparam, lparam);
//
//   }


//   ATOM animated_wallpaper::_s_register_class(HINSTANCE hInstance)
//   {
//
//      wcscpy(s_wszTitle, L"Animated Wallpaper Window"_ansi);
//      wcscpy(s_wszWindowClass, L"animated_wallpaper_window"_ansi);
//
//      WNDCLASSEXW wcex;
//
//      wcex.cbSize = sizeof(WNDCLASSEX);
//
//      wcex.style = CS_HREDRAW | CS_VREDRAW;
//      wcex.lpfnWndProc = WndProc;
//      wcex.cbClsExtra = 0;
//      wcex.cbWndExtra = 0;
//      wcex.hInstance = hInstance;
//      wcex.hIcon = nullptr;
//      wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
//      wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
//      wcex.lpszMenuName = nullptr;
//      wcex.lpszClassName = s_wszWindowClass;
//      wcex.hIconSm = nullptr;
//
//      ATOM atom = RegisterClassExW(&wcex);
//
//      if(atom)
//      {
//
//         s_bClassRegistered = true;
//
//      }
//
//      return atom;
//   }


   void animated_wallpaper::open()
   {

//      m_hinstance = (HINSTANCE)system()->m_hinstanceThis;
//
//      if (!s_bClassRegistered)
//      {
//
//         _s_register_class(m_hinstance);
//
//      }
//
//      m_cx = GetSystemMetrics(SM_CXSCREEN);
//
//      m_cy = GetSystemMetrics(SM_CYSCREEN);
//
//      fork([this]()
//      {
//
//         HWND hwnd  = ::CreateWindowW(s_wszWindowClass, s_wszTitle, 0,
//            0, 0, m_cx, m_cy, nullptr, nullptr, m_hinstance, this);
//
//         MSG msg;
//
//         //ShowWindow(m_hwnd, SW_NORMAL);
//
//         //UpdateWindow(m_hwnd);
//
//         _set_window();
//
//         // Main message loop:
//         while (::task_get_run() && GetMessage(&msg, nullptr, 0, 0))
//         {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//         }
//
//
//      });

   }


   void animated_wallpaper::close()
   {

      //PostMessage(m_hwnd, WM_CLOSE, 0, 0);

   }


//   BOOL CALLBACK find_worker(HWND m_hwnd, LPARAM lp)
//   {
//
//      HWND * pworker = (HWND *)lp;
//
//      if (!FindWindowExA(m_hwnd, 0, "SHELLDLL_DefView"_ansi, 0))
//      {
//
//         return TRUE;
//
//      }
//
//      *pworker = FindWindowExA(0, m_hwnd, "WorkerW"_ansi, 0);
//
//      if (*pworker)
//      {
//
//         return FALSE;
//
//      }
//
//      return TRUE;
//   }
//
//   HWND animated_wallpaper::_get_wallpaper_window()
//   {
//      HWND progman;
//      HWND worker;
//
//      progman = FindWindowA("Progman"_ansi, 0);
//
//      if (!progman)
//      {
//         return 0;
//      }
//
//      /*
//       * this is basically all the magic. it's an undocumented window message that
//       * forces windows to spawn a window with class "WorkerW"_ansi behind deskicons
//       */
//
//      SendMessageA(progman, 0x052C, 0xD, 0);
//      SendMessageA(progman, 0x052C, 0xD, 1);
//
//      EnumWindows(find_worker, (LPARAM)&worker);
//
//      if (!worker)
//      {
//         SendMessageA(progman, 0x052C, 0, 0);
//
//         EnumWindows(find_worker, (LPARAM)&worker);
//      }
//
//      /*
//       * windows 7 with aero is almost the same as windows 10, except that we
//       * have to hide the WorkerW window and render to Progman child windows
//       * instead
//       */
//
//      if (!worker)
//      {
//         worker = progman;
//      }
//
//      return worker;
//   }
//   int animated_wallpaper::_update_window_styles(UINT styleFilter, UINT styleExFilter, UINT styleAdd, UINT styleExAdd)
//   {
//      unsigned gle;
//      long style = 0, exstyle = 0;
//
//      SetLastError(0);
//
//
//      style &= styleFilter;
//      exstyle &= styleExFilter;
//      style |= styleAdd;
//      exstyle |= styleExAdd;
//
//      SetLastError(0);
//
//      if (!SetWindowLongA(m_hwnd, GWL_STYLE, style) ||
//         !SetWindowLongA(m_hwnd, GWL_EXSTYLE, exstyle))
//      {
//         gle = GetLastError();
//         if (gle) {
//            return 1;
//         }
//      }
//      return 0;
//
//   }
//
//
//   void animated_wallpaper::_set_window()
//   {
//      char wndclass[512];
//      HWND hwndWallpaper = _get_wallpaper_window();
//      UINT styleFilter, styleExFilter;
//      int_rectangle r;
//
//      *wndclass = 0;
//      GetClassNameA(m_hwnd, wndclass, sizeof(wndclass) - 1);
//
//      if (hwndWallpaper == m_hwnd || !strcmp(wndclass, "Shell_TrayWnd"_ansi)) {
//         return;
//      }
//
//      if (IsChild(hwndWallpaper, m_hwnd)) {
//         return;
//      }
//
//      /*
//       * styles blacklist taken from https://github.com/Codeusa/Borderless-Gaming/
//       * blob/2fef4ccc121412f215cd7f185c4351fd634cab8b/BorderlessGaming.Logic/
//       * Windows/Manipulation.cs#L70
//       */
//
//       /* TODO: somehow save old styles so we can restore them */
//
//      styleFilter = ~(
//         WS_CAPTION |
//         WS_THICKFRAME |
//         WS_SYSMENU |
//         WS_MAXIMIZEBOX |
//         WS_MINIMIZEBOX
//         );
//
//      styleExFilter = ~(
//         WS_EX_DLGMODALFRAME |
//         WS_EX_COMPOSITED |
//         WS_EX_WINDOWEDGE |
//         WS_EX_CLIENTEDGE |
//         WS_EX_LAYERED |
//         WS_EX_STATICEDGE |
//         WS_EX_TOOLWINDOW |
//         WS_EX_APPWINDOW
//         );
//
//      if (_update_window_styles(styleFilter, styleExFilter, WS_CHILD | WS_VISIBLE, 0))
//      {
//         throw ::exception(error_failed);
//      }
//
//      /* window retains screen coordinates so we need to adjust them */
//      //wp_map_rect(m_hwnd, &r);
//
//      if (!SetParent(m_hwnd, hwndWallpaper))
//      {
//
//         throw ::exception(error_failed);
//      }
//
//      ShowWindow(m_hwnd, SW_SHOW);
//      //wp_move(m_hwnd, r.left(), r.top(), r.right(), r.bottom());
//
//      m_rectParentClient.left() = 0;
//      m_rectParentClient.top() = 0;
//      m_rectParentClient.right() = m_cx;
//      m_rectParentClient.bottom() = m_cy;
//
//      MapWindowPoints(0, hwndWallpaper, (LPPOINT)&m_rectParentClient, 2);
//
//      SetWindowPos(m_hwnd, 0,
//      m_rectParentClient.left(), m_rectParentClient.top(),
//      m_rectParentClient.width(), m_rectParentClient.height(), SWP_NOZORDER | SWP_SHOWWINDOW);
//
//      //return 0;
//   }
//

   void animated_wallpaper::update_wallpaper() 
   {

//   ::RedrawWindow(m_hwnd, nullptr, nullptr, RDW_INVALIDATE);

   }

} // namespace windows



void operating_system_animated_wallpaper_factory_exchange()
{

   factory()->add_factory_item <  ::macos::animated_wallpaper, ::animated_wallpaper>();

}



