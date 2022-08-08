#ifndef TESTENTITY_H
#define TESTENTITY_H

#include"Entity.h"
#include"Renderer.h"

class TestEntity: public Entity, public Transform, public Mesh
{
public:
	TestEntity()
	{
		m_programID = Program_Shader_Managment.GetProgramID("main");

		std::vector<glm::vec3> positions
		{
			 glm::vec3(-0.5f, -0.5f, 0.0f),
			 glm::vec3(0.5f, -0.5f, 0.0f),
			 glm::vec3(-0.5f, 0.5f, 0.0f),
			 glm::vec3(0.5f, 0.5f, 0.0f)
		};

		std::vector<glm::vec2> textureCords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f)
		};

		std::vector<GLuint> elements
		{
			0, 1, 2,
			1, 3, 2

		};

		Helper_::Mesh_::GenerateMesh(*this, positions, textureCords, elements, "Data/Images/Bricks.jpg");
	}

	void Update() override final
	{
		glm::mat4 worldMatrix(1.0f);
		worldMatrix = glm::translate(worldMatrix, glm::vec3(0.25f, 0.25f, 0.0f));
		worldMatrix = glm::rotate(worldMatrix, glm::radians((float)glfwGetTime() * 10), glm::vec3(0.0f, 0.0f, 1.0f));
		worldMatrix = glm::scale(worldMatrix, glm::vec3(-0.5f, 0.5f, 0.5f));

		GLint worldMatLoc{ glGetUniformLocation(m_programID, "worldMat") };
		glUniformMatrix4fv(worldMatLoc, 1, GL_FALSE, glm::value_ptr(worldMatrix));
	}

	void Render(Renderer& renderer) override final
	{
		renderer.RenderMesh(*this);
	}

};

#endif