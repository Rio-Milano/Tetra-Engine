#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include<GLFW/glfw3.h>
#include"glmIncludes.h"

class InputManager
{
public:
	//Input manager is singlton
	static InputManager& GetInstance();

	//assign window ptr and set callback functions for user input
	void InitializeInputManager(GLFWwindow* windowPtr);

	//holds state of last event
	glm::vec2
		m_currentMousePosition;
	float
		m_mouseScrollY;

	//used to prevent large vector when mouse first enters screen
	bool
		m_firstMouseEvent = false,
		m_seccondMouseEvent = false;

private: 
	//singleton
	InputManager() = default;
	~InputManager() = default;

	//internal helpers
	void SetCallBackFunctions(GLFWwindow* windowPtr);

	//gl call back functions
	static void mouse_callback(GLFWwindow* windowPtr, double xpos, double ypos);
	static void scroll_callback(GLFWwindow* windowPtr, double xoffset, double yoffset);

	GLFWwindow* targetWindowPtr;

};




#endif
