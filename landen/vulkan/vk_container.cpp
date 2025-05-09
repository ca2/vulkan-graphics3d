// From vk_window by camilo on 2025-05-09 02:37 <3ThomasBorregaardSorensen!!
#include "framework.h"
#include "vk_container.h"
//#include <GLFW/glfw3.h>
#include <stdexcept>



namespace vkc 
{

	VkContainer::VkContainer() 
	{
		m_bShouldClose = false;
		m_dMouseLastX = 0.;
		m_dMouseLastY = 0.;
		m_bFirstMouse = true;
		m_bLastMouse = false;
		// initWindow();
	}

	VkContainer::~VkContainer() {
		//glfwDestroyWindow(window);
		//glfwTerminate();
	}
	void VkContainer::initWindow() {
		//glfwInit();
		//glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		//window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		//glfwSetWindowUserPointer(window, this);
		//glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}


	::user::enum_key_state VkContainer::get_key_state(::user::e_key ekey)
	{

		return ::user::e_key_state_none;

	}


	bool VkContainer::is_absolute_mouse_position()
	{


		return true;

	}

	//void VkContainer::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface) {
	//	if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
	//		throw std::runtime_error("Failed to create window surface");
	//	}
	//}

	/*void VkContainer::framebufferResizeCallback(GLFWwindow* window, int width, int height) {
		auto Window = reinterpret_cast<VkContainer*>(glfwGetWindowUserPointer(window));
		Window->framebufferResized = true;
		Window->width = width;
		Window->height = height;
	}*/
}