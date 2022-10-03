#ifndef TRANSPARENTENTITY_H
#define TRANSPARRENT_H

#include"Entity.h"

#include<memory>

class Mesh;

class TransparentEntity : public Entity
{
public:
	TransparentEntity(const std::string& entityName = "Transparrent");
	~TransparentEntity() = default;

	void Init() override final;
	void Update(const float& dt) override final;
	void Render(Renderer& renderer, Shader* overideShader) override final;

private:
	std::shared_ptr<Mesh> m_mesh;
};

#endif

