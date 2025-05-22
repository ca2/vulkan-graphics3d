#include "framework.h"



#define WIDTH 256
#define HEIGHT 256
#include "shader_vert.spv.h"  // We'll embed this below
#include "shader_frag.spv.h"  // We'll embed this below
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
   if (msg == WM_DESTROY) PostQuitMessage(0);
   return DefWindowProc(hwnd, msg, wParam, lParam);
}

typedef struct {
   float pos[2];
   float color[3];
} Vertex;

Vertex vertices[] = {
    {{10.0f, 10.0f}, {0.68f, 0.85f, 0.90f}},
    {{110.0f, 10.0f}, {0.68f, 0.85f, 0.90f}},
    {{10.0f, 70.0f}, {0.68f, 0.85f, 0.90f}},
    {{110.0f, 70.0f}, {0.68f, 0.85f, 0.90f}}
};

uint16_t indices[] = {
    0, 1, 2,
    2, 1, 3
};

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpCmd, int nShow) {
   // Register window class
   WNDCLASS wc = { 0 };
   wc.lpfnWndProc = WndProc;
   wc.hInstance = hInst;
   wc.lpszClassName = L"VulkanLayeredWindow";
   RegisterClass(&wc);

   // Create layered window
   HWND hwnd = CreateWindowEx(WS_EX_LAYERED, 
      L"VulkanLayeredWindow", 
      L"Vulkan Rectangle", WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, CW_USEDEFAULT, WIDTH, HEIGHT, NULL, NULL, hInst, NULL);

   ShowWindow(hwnd, nShow);

   // Vulkan initialization would go here:
   // - Instance
   // - Physical and logical device
   // - Offscreen image, render pass, framebuffer
   // - Shader module creation from shader_vert_spv and shader_frag_spv
   // - Pipeline layout and graphics pipeline
   // - Vertex/index buffers
   // - Command buffer recording (clear to white and draw blue rectangle)
   // - Submit rendering and copy image to host memory
   // - Convert image to DIB and call UpdateLayeredWindow

   // Message loop
   MSG msg = { 0 };
   while (GetMessage(&msg, NULL, 0, 0)) {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
   }

   return 0;
}
