#include "../H/BaseGrid.h"
#include"../../Core/Graphics/H/Renderer.h"

void BaseGrid::Init()
{
	for (size_t z = 0ull; z < baseMeshZCells + 1; z++)
	{
		std::shared_ptr<Line> newLine = std::make_shared<Line>("");
		newLine->AddVertex(glm::vec3(left_x, 0.0f, top_z + z * baseMeshZCellDepth), baseMeshColor);
		newLine->AddVertex(glm::vec3(right_x, 0.0f, top_z + z * baseMeshZCellDepth), baseMeshColor);
		newLine->Init();
		m_entities.emplace_back(newLine);
	}
	for (size_t x = 0ull; x < baseMeshXCells + 1; x++)
	{
		std::shared_ptr<Line> newLine = std::make_shared<Line>("");
		newLine->AddVertex(glm::vec3(left_x + x * baseMeshXCellWidth, 0.0f, top_z), baseMeshColor);
		newLine->AddVertex(glm::vec3(left_x + x * baseMeshXCellWidth, 0.0f, bottom_z), baseMeshColor);
		newLine->Init();
		m_entities.emplace_back(newLine);
	}

	//axis notation visual
	const std::shared_ptr<Line>& originLine = std::make_shared<Line>("OriginLine");
	originLine->AddVertex(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	originLine->AddVertex(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	originLine->AddVertex(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	originLine->AddVertex(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	originLine->AddVertex(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	originLine->AddVertex(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	originLine->Init();
	originLine->GetRoot()->SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	m_entities.emplace_back(originLine);
}

void BaseGrid::Render(Renderer& renderer, Shader* overideShader)
{
	Shader* shader = overideShader ? overideShader : &ShaderManager.GetShader("main");

	for (const std::shared_ptr<Entity>& entity : m_entities)
	{
		entity->Render(renderer, shader);
	}
}
