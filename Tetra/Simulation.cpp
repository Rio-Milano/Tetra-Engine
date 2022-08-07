#include "Simulation.h"

Simulation::Simulation(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, UpdateViewportWhenWindowSizeChanged);
}

void Simulation::Update(GLFWwindow* window)
{
	ProcessInput(window);
	glfwPollEvents();
}



void Simulation::UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight)
{
	glViewport(0, 0, windowWidth, windowHeight);
}


void Simulation::ProcessInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}
