#ifndef LINE_H
#define LINE_H


#include"Entity.h"
#include"Renderer.h"
#include"Model.h"
#include<iostream>


class Line : public Entity, public Model
{
public:
	Line() = default;
	~Line() = default;

	void AddVertex(const glm::vec3& position, const glm::vec3& color = glm::vec3(1.0f))
	{
		m_verticies.emplace_back(position);
		m_verticies.emplace_back(color);
	};

	void Init() override final
	{
		std::shared_ptr<Mesh> newMesh = std::make_shared<Mesh>();
		
		glGenVertexArrays(1, &newMesh->m_VAO);
		glBindVertexArray(newMesh->m_VAO);

		//send line data to gpu
		newMesh->CreateBuffer(GL_ARRAY_BUFFER, m_verticies.size() * sizeof(glm::vec3), m_verticies.data(), GL_STATIC_DRAW);
		
		//setup vertex stream for position
		newMesh->CreateVertexAttributePointer(GL_ARRAY_BUFFER, 0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3)*2, 0);

		//setup vertex stream for color
		newMesh->CreateVertexAttributePointer(GL_ARRAY_BUFFER, 1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3) * 2, (void*)sizeof(glm::vec3));

		//configure mesh
		newMesh->m_programName = "Position-Color";
		newMesh->m_meshName = "Line";
		newMesh->m_drawType = 2;
		newMesh->customVertex = true;
		newMesh->m_vertexCount = m_verticies.size() / 2ull;
		

		//finished with mesh
		glBindVertexArray(0);

		std::shared_ptr<ModelNode> newModelNode = std::make_shared<ModelNode>(glm::mat4(1.0f), "line");
		newModelNode->AddMesh(newMesh);

		m_rootModelNode = newModelNode;

	}

	void Update(const float& dt) override final {};

	void Render(Renderer& renderer)override final
	{
		m_rootModelNode->Render(renderer);
	}

private:

	std::vector<glm::vec3> m_verticies;
};

#endif