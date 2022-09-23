#ifndef PLANE_H
#define PLANE_H

#include"Entity.h"
#include"Model.h"

class Plane : public Entity, private Model
{
public:
	Plane(const std::string& entityName = "Plane");
	~Plane() = default;


	void Init() override final;
	void Update(const float& dt) override final;
	void Render(Renderer& renderer)override final;

};

#endif