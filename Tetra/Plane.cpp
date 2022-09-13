#include "Plane.h"
#include"Renderer.h"
#include<vector>

Plane::Plane(const std::string& entityName) 
	: Entity(entityName) 
{

}



void Plane::Init()
{
	std::vector<glm::vec3> positions
	{
		glm::vec3(-1.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f)
	};

	std::vector<glm::vec3> colors
	{
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f),
		glm::vec3(1.0f)
	};

	std::vector<unsigned int> elements
	{
		2, 1, 0,
		2, 3, 1
	};

	std::shared_ptr<Mesh> planeMesh = std::make_shared<Mesh>();
	
	glGenVertexArrays(1, &planeMesh->m_VAO);
	glBindVertexArray(planeMesh->m_VAO);

	planeMesh->CreateBuffer(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);
	planeMesh->CreateVertexAttributePointer(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	planeMesh->CreateBuffer(GL_ARRAY_BUFFER, colors.size() * sizeof(glm::vec3), colors.data(), GL_STATIC_DRAW);
	planeMesh->CreateVertexAttributePointer(GL_ARRAY_BUFFER, 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

	planeMesh->m_elements = &elements;
	planeMesh->CreateBuffer(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(unsigned int), elements.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
	planeMesh->EndBuffer(GL_ELEMENT_ARRAY_BUFFER);

	//configure mesh
	planeMesh->m_programName = "Position-Color";
	planeMesh->m_meshName = "Plane";
	planeMesh->m_drawType = 0;


	std::shared_ptr<ModelNode> modelNode = std::make_shared<ModelNode>(glm::mat4(1.0f));
	modelNode->AddMesh(planeMesh);
	SetRoot(modelNode);

}

void Plane::Update(const float& dt)
{
}

void Plane::Render(Renderer& renderer)
{
	GetRoot()->Render(renderer);
}
