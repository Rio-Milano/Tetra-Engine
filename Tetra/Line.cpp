#include "Line.h"

void Line::AddVertex(const glm::vec3& position, const glm::vec3& color)
{
	m_verticies.emplace_back(position);
	m_verticies.emplace_back(color);
};

void Line::Init()
{
	//create a mesh
	std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>();

	//gen vertex array
	glGenVertexArrays(1, &newMesh->m_VAO);
	//bind to vertex array
	glBindVertexArray(newMesh->m_VAO);

	//send line data to gpu
	newMesh->CreateBuffer(GL_ARRAY_BUFFER, m_verticies.size() * sizeof(glm::vec3), m_verticies.data(), GL_STATIC_DRAW);

	//setup vertex stream for position
	newMesh->CreateVertexAttributePointer(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, 0);

	//setup vertex stream for color
	newMesh->CreateVertexAttributePointer(GL_ARRAY_BUFFER, 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)sizeof(glm::vec3));

	//configure mesh
	newMesh->m_programName = "Position-Color";
	newMesh->m_meshName = "Line";
	newMesh->m_drawType = 2;
	newMesh->customVertex = true;
	newMesh->m_vertexCount = m_verticies.size() / 2ull;


	//unbind from vao
	glBindVertexArray(0);

	//create a root node for model
	std::shared_ptr<ModelNode> newModelNode = std::make_shared<ModelNode>(glm::mat4(1.0f), "line");
	//add created mesh to root node
	newModelNode->AddMesh(newMesh);
	//set root node of model to new root
	SetRoot(newModelNode);

}

void Line::Render(Renderer& renderer)
{
	//render model from root
	GetRoot()->Render(renderer);
}