#include "TransparentEntity.h"
#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()
#include"Texture.h"
#include"Mesh.h"
#include"Renderer.h"

TransparentEntity::TransparentEntity(const std::string& entityName)
	:Entity(entityName)
{
	if (TextureManager.GetTexture("Window") == nullptr)
	{
		std::shared_ptr<Texture> newTexture = std::make_shared<Texture>();
		newTexture->InitializeTexture("Data/Images/Window.png", true);
		TextureManager.AddTexture("Window", newTexture);
	}
}

void TransparentEntity::Init()
{
	std::vector<glm::vec3>* positions = new std::vector<glm::vec3>
	{
		glm::vec3(-1.0f, 2.0f, 0.0f),//top left 0
		glm::vec3(1.0f, 2.0f, 0.0f),//top right 1
		glm::vec3(-1.0f, 0.0f, 0.0f),//bottom left 2
		glm::vec3(1.0f, 0.0f, 0.0f),//bottom right 3
	};



	std::vector<glm::vec2>* textureCoords = new std::vector<glm::vec2> 
	{
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
	};

	std::vector<unsigned int>* elements = new std::vector<unsigned int>
	{
		2, 1, 0,
		2, 3, 1
	};

	this->m_mesh = std::make_shared<Mesh>();
	this->m_mesh->GenerateMesh(positions, {}, textureCoords, elements, 0, GL_STATIC_DRAW, "DiscardAlpha");
	

	this->m_mesh->GetMaterial()->m_diffuse = TextureManager.GetTexture("Window");
	this->m_mesh->GetMaterial()->m_discardLowAlphaFragments = true;
	this->m_mesh->GetMaterial()->m_blendingEnabled = true;
	this->m_mesh->SetFaceCullingFlag(false);//dont cull back faces as not closed box
}

void TransparentEntity::Update(const float& dt)
{
}

void TransparentEntity::Render(Renderer& renderer)
{
	static const std::vector<glm::vec3> windowPositions
	{
		glm::vec3(-10.0f, 0.0f, 0.0f),
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -10.0f),
		glm::vec3(0.0f, 0.0f, 10.0f)
	};

	for (const glm::vec3& pos : windowPositions)
	{
		glm::mat4 transform(1.0f);
		transform = glm::translate(transform, pos);
		renderer.RenderMesh(*this->m_mesh.get(), transform);

	}

	glm::mat4 transform(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 1.0f));
	renderer.RenderMesh(*this->m_mesh.get(), transform);
	
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -1.0f));
	renderer.RenderMesh(*this->m_mesh.get(), transform);
	
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(-1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	renderer.RenderMesh(*this->m_mesh.get(), transform);
	
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	renderer.RenderMesh(*this->m_mesh.get(), transform);


}
