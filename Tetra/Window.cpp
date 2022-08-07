#include "Window.h"

#include<iostream>
#include<string>

Window::~Window()
{
	glfwTerminate();
}

bool Window::CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	m_renderWindow = glfwCreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName.c_str(), 0, 0);

	if (!m_renderWindow)
	{
		std::cout << "Failed to create an openGL window with GLFW!\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_renderWindow);


	return true;
}


GLFWwindow* Window::GetWindowPtr()
{
	return m_renderWindow;
}