#ifndef ENGINE_H
#define ENGINE_H

#include"Renderer.h"
#include"Simulation.h"

class Engine
{
public:
	Engine();
	~Engine() = default;

	void Run();

private:
	Renderer m_renderer;
	Simulation m_simulation;
};

#endif