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

bool Window::CreateWindowGLFW(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//create window with passed size, name and also opens on primary monitor on full screen
	//when in debugging mode full screen will cause issues with input so do not make fullscreen in debug mode
	#if _DEBUG
		m_renderWindow = glfwCreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName.c_str(), 0, 0);
	#else
		m_renderWindow = glfwCreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName.c_str(), glfwGetPrimaryMonitor(), 0);
	#endif

	if (!m_renderWindow)
	{
		std::cout << "Failed to create an openGL window with GLFW!\n";
		glfwTerminate();
		return false;
	}

	//if window creation was successful then set that window to the window glfw should refer to
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
	//opengl 3.3 core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);//set major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//set minor version to 3
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);//tell opengl to use modern standard
}