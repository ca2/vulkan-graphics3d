//#pragma once
//#define GLFW_INCLUDE_VULKAN
////#include <GLFW/glfw3.h>
//#include <stdexcept>
//
//
//namespace graphics3d_vulkan {
//
//	class VkWindow {
//	public:
//		VkWindow(int w, int h, std::string name);
//		~VkWindow();
//
//		VkWindow(const VkWindow&) = delete;
//		VkWindow& operator=(const VkWindow&) = delete;
//
//		//bool shouldClose() { return glfwWindowShouldClose(window); }
//		bool shouldClose() { return false; }
//		::int_size getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; }
//		bool wasWindowResized() { return framebufferResized; }
//		void resetWindowResizedFlag() { framebufferResized = false; }
//		//GLFWwindow* getGLFWwindow() const { return window; }
//		::windowing::window * get_windowing_window() const { return nullptr; }
//
//
//		//void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
//	private:
//
//		//static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
//		void initWindow();
//
//		int width;
//		int height;
//		bool framebufferResized = false;
//
//
//		std::string windowName;
//		//GLFWwindow* window;
//		::windowing::window* m_pwindow;
//	};
//
//
//
//}