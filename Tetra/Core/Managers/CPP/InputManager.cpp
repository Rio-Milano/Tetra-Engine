#include "../H/InputManager.h"

InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::InitializeInputManager(GLFWwindow* windowPtr)
{
    targetWindowPtr = windowPtr;
    SetCallBackFunctions(windowPtr);    
}

void InputManager::SetCallBackFunctions(GLFWwindow* windowPtr)
{
    glfwSetCursorPosCallback(windowPtr, mouse_callback);
    glfwSetScrollCallback(windowPtr, scroll_callback);
}

void InputManager::mouse_callback(GLFWwindow* windowPtr, double xpos, double ypos)
{
    InputManager& inputManager = InputManager::GetInstance();
    
    if (inputManager.m_firstMouseEvent)
        inputManager.m_seccondMouseEvent = true;
    else 
        inputManager.m_firstMouseEvent = true;

    inputManager.m_currentMousePosition = glm::vec2(static_cast<float>(xpos), static_cast<float>(ypos));
}

void InputManager::scroll_callback(GLFWwindow* windowPtr, double xoffset, double yoffset)
{
    InputManager::GetInstance().m_mouseScrollY = static_cast<float>(yoffset);
}

