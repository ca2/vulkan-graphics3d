// From vk_window by camilo on 2025-05-09 02:32 <3ThomasBorregaardSorensen!!
#pragma once
//#define GLFW_INCLUDE_VULKAN
//#include <GLFW/glfw3.h>
//#include <stdexcept>
//#include <vulkan/vulkan.h>
namespace user
{

	enum enum_key_state
	{

		e_key_state_none,
		e_key_state_pressed = 1,

	};

}

namespace vkc 
{

	
	class CLASS_DECL_vulkan_land_LANDEN VkContainer :
		virtual public ::particle
	{
	public:


		::function < void(void*, int, int, int)> m_callbackOffscreen;

		double   m_dCursorX;
		double   m_dCursorY;
		bool		m_bFirstMouse;
		bool		m_bLastMouse;


		bool		m_bShouldClose;

		
		VkContainer();
		~VkContainer();

		VkContainer(const VkContainer&) = delete;
		VkContainer& operator=(const VkContainer&) = delete;
		virtual bool is_absolute_mouse_position();

		//bool shouldClose() { return glfwWindowShouldClose(window); }
		bool shouldClose() { return false; }
		virtual ::int_size size() = 0;
		bool wasWindowResized() { return framebufferResized; }
		void resetWindowResizedFlag() { framebufferResized = false; }
		//GLFWwindow* getGLFWwindow() const { return window; }
		//::windowing::window * get_windowing_window() const { return nullptr; }

		virtual ::user::enum_key_state get_key_state(::user::e_key ekey);

		//void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
	public:

		//static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		void initWindow();

		int m_width;
		int m_height;
		bool framebufferResized = false;


		//std::string windowName;
		//GLFWwindow* window;
		//::windowing::window* m_pwindow;
	};



}