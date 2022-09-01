#ifndef CONTAINER_H
#define CONTAINER_H

#include"Entity.h"
#include"Renderer.h"
#include"Model.h"
#include<iostream>

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

class Container: public Entity, public Model
{
public:

	Container(const std::string& containerName)
		:Entity(containerName)
	{
		if (TextureManager.GetTexture("box") == nullptr)
		{
			std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>();
			diffuseTexture->InitializeTexture("Data/Images/Box.png");
			TextureManager.AddTexture("Box", diffuseTexture);
		}

		if (TextureManager.GetTexture("BoxSpec") == nullptr)
		{
			std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>();
			specularTexture->InitializeTexture("Data/Images/BoxSpec.png");
			TextureManager.AddTexture("BoxSpec", specularTexture);
		}

		if (TextureManager.GetTexture("BoxEmission") == nullptr)
		{
			std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>();
			emissionTexture->InitializeTexture("Data/Images/BoxEmission.png");
			TextureManager.AddTexture("BoxEmission", emissionTexture);
		}

	};
	~Container() = default;

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

		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
		mesh->GenerateMesh(positions, normals, textureCords, {}, "Box", "BoxSpec", "BoxEmission", 1);


		std::shared_ptr<ModelNode> modelNode_1 = std::make_shared<ModelNode>(glm::mat4(1.0f), "box");
		modelNode_1->AddMesh(mesh);


		SetRoot(modelNode_1);

	}

	float rot = 0.0f;
	void Update(const float& dt) override final
	{
		rot += dt;
	}

	void Render(Renderer& renderer) override final
	{
		for (int i = 0; i < cubePositions.size(); i++)
		{
			GetRoot()->SetTransform(glm::rotate(glm::translate(glm::mat4(1.0f), cubePositions[i] * 2.0f), rot, glm::vec3(1.0f)));
			GetRoot()->Render(renderer);
		}
	}

	private:
		std::vector<glm::vec3> cubePositions = {
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
};

#endif