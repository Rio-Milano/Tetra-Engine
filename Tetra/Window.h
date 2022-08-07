#ifndef WINDOW_H
#define WINDOW_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include<string>

class Window
{
public:
	Window() = default;
	~Window();

	bool CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName);
	GLFWwindow* GetWindowPtr();

private:
	
	GLFWwindow* m_renderWindow{ nullptr };
};

#endif