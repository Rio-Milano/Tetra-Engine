#ifndef GRASS_H
#define GRASS_H

#include"Entity.h"

#include<memory>

class Mesh;


class Grass : public Entity
{
public:
	Grass(const std::string& entityName = "Grass");
	~Grass() = default;

	void Init() override final;
	void Update(const float& dt) override final;
	void Render(Renderer& renderer) override final;

private: 
	std::shared_ptr<Mesh> m_mesh;
};

#endif