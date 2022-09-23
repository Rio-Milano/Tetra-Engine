#ifndef WINDOW_H
#define WINDOW_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include"../../Requirements/glmIncludes.h"
#include<string>

class Window
{
public:
	Window();
	~Window();

	bool CreateWindowGLFW(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName);
	GLFWwindow* GetWindowPtr();
	void UpdateWindow();
	
	glm::vec<2, int> GetWindowSize()const;

private:
	//internal helper funstions
	void Set_GL_Context();

	GLFWwindow* m_renderWindow{ nullptr };
};

#endif