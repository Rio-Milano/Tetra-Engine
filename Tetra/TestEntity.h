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
			glm::vec3( - 0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(- 0.5f, 0.5f, -0.5f), 
			glm::vec3(-0.5f, -0.5f, -0.5f), 
			glm::vec3(-0.5f, -0.5f, 0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f) , 
			glm::vec3(0.5f, 0.5f, 0.5f), 
			glm::vec3(0.5f, 0.5f, 0.5f), 
			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, -0.5f, 0.5f), 
			glm::vec3(-0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f), 
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(-0.5f, -0.5f, 0.5f), 
			glm::vec3(-0.5f, 0.5f, 0.5f), 
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f), 
			glm::vec3(0.5f, -0.5f, -0.5f), 
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, 0.5f), 
			glm::vec3(0.5f, 0.5f, 0.5f), 
			glm::vec3(-0.5f, -0.5f, -0.5f), 
			glm::vec3(0.5f, -0.5f, -0.5f), 
			glm::vec3(0.5f, -0.5f, 0.5f), 
			glm::vec3(0.5f, -0.5f, 0.5f), 
			glm::vec3(-0.5f, -0.5f, 0.5f), 
			glm::vec3(-0.5f, -0.5f, -0.5f), 
			glm::vec3(-0.5f, 0.5f, -0.5f), 
			glm::vec3(0.5f, 0.5f, -0.5f), 
			glm::vec3(0.5f, 0.5f, 0.5f), 
			glm::vec3(0.5f, 0.5f, 0.5f),
			glm::vec3(-0.5f, 0.5f, 0.5f), 
			glm::vec3(-0.5f, 0.5f, -0.5f), 
		};

		std::vector<glm::vec2> textureCords
		{
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(0.0f, 1.0f),
			glm::vec2(1.0f, 1.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(1.0f, 0.0f),
			glm::vec2(0.0f, 0.0f),
			glm::vec2(0.0f, 1.0f)

		};

		Helper_::Mesh_::GenerateMesh(*this, positions, textureCords, "Data/Images/Bricks.jpg", 1);
	}

	void Update() override final
	{
		//m_transform = glm::mat4(1.0f);
		//m_transform = glm::translate(m_transform, glm::vec3(0.25f, 0.25f, 0.0f));
		m_transform = glm::rotate(m_transform, glm::radians(sin(0.1f)), glm::vec3(1.0f, 1.0f, 1.0f));
		//m_transform = glm::scale(m_transform, glm::vec3(0.9f, 0.9f, 0.9f));


		GLint worldMatLoc{ glGetUniformLocation(m_programID, "worldMat") };
		glUniformMatrix4fv(worldMatLoc, 1, GL_FALSE, glm::value_ptr(m_transform));
	}

	void Render(Renderer& renderer) override final
	{
		renderer.RenderMesh(*this);
	}

};

#endif