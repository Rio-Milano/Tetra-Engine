#ifndef ENGINE_H
#define ENGINE_H

#include"Renderer.h"
#include"Simulation.h"
#include"Entity.h"

#include<memory>
#include<vector>

class Engine
{
public:
	Engine() = default;;
	~Engine() = default;

	void Run();

private:
	void InitGLAD();

	std::vector<std::shared_ptr<Entity>> m_entities;
	Renderer m_renderer;
	Simulation m_simulation;



};

#endif