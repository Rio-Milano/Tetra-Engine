#include "Simulation.h"
#include"Entity.h"


void Simulation::InitSimulation(GLFWwindow* window)
{
	glfwSetFramebufferSizeCallback(window, UpdateViewportWhenWindowSizeChanged);
}

void Simulation::Update(GLFWwindow* window)
{
	ProcessInput(window);
	glfwPollEvents();
}

void Simulation::UpdateEntity(Entity& entity)
{
	entity.Update();
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
