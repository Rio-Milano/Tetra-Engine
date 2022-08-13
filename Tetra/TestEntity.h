#ifndef TESTENTITY_H
#define TESTENTITY_H

#include"Entity.h"
#include"Renderer.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"Mesh.h"

class TestEntity: public Entity, public Transform, public Mesh
{
public:
	TestEntity() = default;
	void Init() override final 
	{
		std::vector<glm::vec3> positions
		{
			glm::vec3(-0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, -0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(0.5f, 0.5f, -0.5f),
			glm::vec3(-0.5f, 0.5f, -0.5f),
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

		std::vector<glm::vec3> normals
		{
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(-1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, -1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		};

		GenerateMesh(*this, positions, normals, textureCords,"Bricks", 1);
		SetProgramID(ShaderManager.GetShader("main").GetID());
		//m_transform = glm::translate(m_transform, glm::vec3(0.f, 0.f, -10.f));
	}


	void Update() override final
	{
		//m_transform = glm::mat4(1.0f);
		//m_transform = glm::translate(m_transform, glm::vec3(0.25f, 0.25f, 0.0f));
		m_transform = glm::rotate(m_transform, glm::radians(sin(1.f)), glm::vec3(1.0f, 0.0f, 0.0f));
		//m_transform = glm::scale(m_transform, glm::vec3(2.f, 2.f, 2.f));


		GLint worldMatLoc{ glGetUniformLocation(ShaderManager.GetShader("main").GetID(), "worldMat")};
		glUniformMatrix4fv(worldMatLoc, 1, GL_FALSE, glm::value_ptr(m_transform));
	}

	void Render(Renderer& renderer) override final
	{
		renderer.RenderMesh(*this);
	}

};

#endif