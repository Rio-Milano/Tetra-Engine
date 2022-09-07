#include "Grass.h"

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"Texture.h"
#include"Mesh.h"
#include"Renderer.h"

Grass::Grass(const std::string& entityName)
	:Entity(entityName)
{
	if (TextureManager.GetTexture("Grass") == nullptr)
	{
		std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
		newTexture->InitializeTexture("Data/Images/Grass.png", true);
		TextureManager.AddTexture("Grass", newTexture);
	}
}

void Grass::Init()
{
	std::vector<glm::vec3> positions
	{
		glm::vec3(-1.0f, 2.0f, 0.0f),//top left 0
		glm::vec3(1.0f, 2.0f, 0.0f),//top right 1
		glm::vec3(-1.0f, 0.0f, 0.0f),//bottom left 2
		glm::vec3(1.0f, 0.0f, 0.0f),//bottom right 3
	};



	std::vector<glm::vec2> textureCoords
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
	};

	std::vector<unsigned int> elements
	{
		2, 1, 0,
		2, 3, 1
	};

	this->m_mesh = std::make_shared<Mesh>();
	this->m_mesh->GenerateMesh(positions, {}, textureCoords, elements, 0, GL_STATIC_DRAW, "DiscardAlpha");
	this->m_mesh->GetMaterial()->m_diffuse = TextureManager.GetTexture("Grass");
	this->m_mesh->GetMaterial()->m_discardLowAlphaFragments = true;
	this->m_mesh->SetFaceCullingFlag(false);
}

void Grass::Update(const float& dt)
{
}

void Grass::Render(Renderer& renderer)
{
	static std::vector<glm::vec3> grassPositions
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 10.0f),
		glm::vec3(5.0f, 0.0f, 0.0f)
	};

	static std::vector<glm::vec3> grassScaling
	{
		glm::vec3(1.0f, 1.0f, 1.0f),
		glm::vec3(2.0f, 1.0f, 2.0f),
		glm::vec3(1.0f, 1.5f, 1.0f),
		glm::vec3(1.0f, 1.5f, 1.0f)
	};

	static float theta = 0.0f;

	for (size_t i = 0ull; i < grassPositions.size(); i++)
	{
		const glm::vec3& position = grassPositions[i];
		const glm::vec3& scale = grassScaling[i];

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position);

		glm::mat4 t1 = glm::scale(transform, scale);
		t1 = glm::rotate(t1, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		renderer.RenderMesh(*m_mesh.get(), t1);

		glm::mat4 t2 = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		t2 = glm::scale(t2, scale);
		t2 = glm::rotate(t2, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		renderer.RenderMesh(*m_mesh.get(), t2);

		glm::mat4 t3 = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		t3 = glm::scale(t3, scale);
		t3 = glm::rotate(t3, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		renderer.RenderMesh(*m_mesh.get(), t3);

		glm::mat4 t4 = glm::rotate(transform, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		t4 = glm::scale(t4, scale);
		t4 = glm::rotate(t4, glm::radians(theta), glm::vec3(0.0f, 1.0f, 0.0f));
		renderer.RenderMesh(*m_mesh.get(), t4);
	}
	theta += static_cast<float>(sin(glfwGetTime()) / 2.0 + 0.5);
	if (theta >= 360.0f)
		theta = 0.0f;
};
