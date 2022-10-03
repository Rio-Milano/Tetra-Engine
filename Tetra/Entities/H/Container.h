#ifndef CONTAINER_H
#define CONTAINER_H

#include"Entity.h"
#include"../../Core/Graphics/H/Renderer.h"
#include"../../Core/Structure/H/Model.h"

#include<iostream>

#include"../../Core/Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()

class Container: public Entity, public Model
{
public:

	Container(const std::string& containerName);
	~Container() = default;

	void Init() override final;

	void Update(const float& dt) override final;

	void Render(Renderer& renderer, Shader* overrideShader) override final;

	private:
		std::vector<glm::vec3> cubePositions = 
		{
			glm::vec3(-8.0f, 0.0f, 0.0f),
			glm::vec3(5.0f, 5.0f, -15.0f),
			glm::vec3(-7.5f, -2.2f, -2.5f),
			glm::vec3(-3.8f, -2.0f, -12.3f),
			glm::vec3(-5.4f, -0.4f, -3.5f),
			glm::vec3(-4.7f, 3.0f, -7.5f),
			glm::vec3(9.3f, -2.0f, -2.5f),
			glm::vec3(6.5f, 2.0f, -2.5f),
			glm::vec3(-6.5f, 0.2f, -1.5f),
			glm::vec3(-4.3f, 1.0f, -1.5f)
		};

		float m_rot = 0.0f;
};

#endif