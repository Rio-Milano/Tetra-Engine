#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include<GLFW/glfw3.h>
#include"glmIncludes.h"

class InputManager
{
public:
	static InputManager& GetInstance();

	void InitializeInputManager(GLFWwindow* windowPtr);

	glm::vec2
		m_currentMousePosition;
	float
		m_mouseScrollY;

private: 
	InputManager() = default;
	~InputManager() = default;

	void SetCallBackFunctions(GLFWwindow* windowPtr);

	static void mouse_callback(GLFWwindow* windowPtr, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* windowPtr, double xoffset, double yoffset);

	GLFWwindow* targetWindowPtr;

};




#endif
