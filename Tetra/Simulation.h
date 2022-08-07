#ifndef SIMULATION_H
#define SIMULATION_H

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>

class Simulation
{
public:
	Simulation(GLFWwindow* window);
	~Simulation() = default;

	void Update(GLFWwindow* window);

private:
	static void UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight);
	static void ProcessInput(GLFWwindow* window);
};

#endif