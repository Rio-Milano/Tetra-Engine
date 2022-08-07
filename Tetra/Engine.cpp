#include "Engine.h"


#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>

Engine::Engine()
	:m_renderer(800, 600, "Tetra"), m_simulation(m_renderer.GetWindow().GetWindowPtr())
{

}

void Engine::Run()
{
	static GLFWwindow* window{ m_renderer.GetWindow().GetWindowPtr() };
	
	while (!glfwWindowShouldClose(window))
	{
		m_simulation.Update(window);
		m_renderer.Render();
	}
}