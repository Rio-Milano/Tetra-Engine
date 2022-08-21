#include "Window.h"

#include<iostream>
#include<string>

Window::Window()
{
	//initialize glfw
	glfwInit();

	//set opengl context
	Set_GL_Context();
}

Window::~Window()
{
	glfwTerminate();
}

bool Window::CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	m_renderWindow = glfwCreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName.c_str(), glfwGetPrimaryMonitor(), 0);

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

void Window::UpdateWindow()
{
	glfwPollEvents();

	if (glfwGetKey(m_renderWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(m_renderWindow, true);
}

void Window::Set_GL_Context()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set minor version to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//tell opengl to use modern standard
}