#ifndef SIMULATION_H
#define SIMULATION_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
class Entity;

class Simulation
{
public:
	Simulation() = default;
	~Simulation() = default;

	void InitSimulation(GLFWwindow* window);
	void Update(GLFWwindow* window);
	void UpdateEntity(Entity& entity);

private:
	static void UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight);
	static void ProcessInput(GLFWwindow* window);
};

#endif