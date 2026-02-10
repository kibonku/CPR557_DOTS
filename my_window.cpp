#include "my_window.h"
#include "my_application.h"
#include <stdexcept>
#include <iostream>

MyWindow::MyWindow(int w, int h, std::string name) : 
	m_iWidth(w),
	m_iHeight(h),
	m_sWindowName(name),
	m_pWindow(nullptr),
	m_pMyApplication(nullptr)
{
	_initWindow();
}

MyWindow::~MyWindow()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void MyWindow::s_keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // TODO: Implement reset view
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        auto mywindow = reinterpret_cast<MyWindow*>(glfwGetWindowUserPointer(window));
        std::cout << "Reset view!" << std::endl;

        if (mywindow && mywindow->m_pMyApplication)
        {
            mywindow->m_pMyApplication->clearPoints();
        }
    }
	// Phase 4: remove last point
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		auto mywindow = reinterpret_cast<MyWindow*>(glfwGetWindowUserPointer(window));
		if (mywindow && mywindow->m_pMyApplication)
		{
			mywindow->m_pMyApplication->removeLastPoint();
			std::cout << "Remove last point (D)!" << std::endl;
		}
	}

}


void MyWindow::s_mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	//*Add: Phase 3
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		auto mywindow = reinterpret_cast<MyWindow*>(glfwGetWindowUserPointer(window));
		double xpos = 0.0, ypos = 0.0;
		glfwGetCursorPos(window, &xpos, &ypos);

		mywindow->setLButtonPress((float)xpos, (float)ypos);
	}
}

void MyWindow::_initWindow()
{
	glfwInit();

	// GLFW was designed initially to create OpenGL context by default.
	// By setting GLFW_CLIENT_API to GLFW_NO_API, it tells GLFW NOT to create OpenGL context 
	// (because we are going to use Vulkan)
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	// Need to handle the window resizing in a specical way later in Vulkan code
	// so we need to set it to false here for now
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	m_pWindow = glfwCreateWindow(m_iWidth, m_iHeight, m_sWindowName.c_str(), nullptr, nullptr);

	// For the call back function to use this pointer
	glfwSetWindowUserPointer(m_pWindow, this);

    // Register keyboard callback	
	glfwSetKeyCallback(m_pWindow, s_keyboardCallback);

    // Register mouse button callback
	glfwSetMouseButtonCallback(m_pWindow, s_mouseButtonCallback);
}

void MyWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(instance, m_pWindow, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create window");
	}
}

void MyWindow::pollEvents()
{
	glfwPollEvents();

    // TODO: When pressing the spacebar GLFW_KEY_SPACE, reset the view window
}

void MyWindow::bindMyApplication(MyApplication* pMyApplication)
{
	m_pMyApplication = pMyApplication;
}

void MyWindow::setLButtonPress(float x, float y)
{
	// TODO: Add additional vertex based on the mouse input
	// Note: x and y are in Windows coordinates (in pixels)
	if (m_pMyApplication != nullptr)
	{
		m_pMyApplication->addPoint(x, y);
	}
}

