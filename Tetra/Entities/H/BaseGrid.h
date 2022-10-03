#ifndef BASEGRID_H
#define BASEGRID_H

#include"Entity.h"
#include"Line.h"
#include<vector>

class BaseGrid : public Entity
{
public:
	BaseGrid(const std::string& entityName = "BaseGrid") : Entity(entityName) {};
	~BaseGrid() = default;


	void Init() override final;
	void Update(const float& dt) override final {};
	void Render(Renderer& renderer, Shader* overrideShader)override final;

private:
	const glm::vec3 baseMeshColor{ 1.0f };
	const size_t baseMeshXCells{ 40ull };
	const float baseMeshXCellWidth{ 2.0f };
	const size_t baseMeshZCells{ 40ull };
	const float baseMeshZCellDepth{ 2.0f };
	const float left_x{ -(baseMeshXCells * baseMeshXCellWidth) / 2.0f };
	const float right_x{ (baseMeshXCells * baseMeshXCellWidth) / 2.0f };
	const float top_z{ -(baseMeshZCells * baseMeshZCellDepth) / 2.0f };
	const float bottom_z{ (baseMeshZCells * baseMeshZCellDepth) / 2.0f };

	std::vector<std::shared_ptr<Entity>> m_entities;
};

#endif

