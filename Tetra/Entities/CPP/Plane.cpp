#include "../H/Plane.h"
#include"../../Core/Graphics/H/Renderer.h"
#include<vector>

Plane::Plane(const std::string& entityName) 
	: Entity(entityName) 
{

}



void Plane::Init()
{
	std::vector<glm::vec3>* positions = new std::vector<glm::vec3>
	{
		glm::vec3(-1.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f),
		glm::vec3(1.0f, 0.0f, 1.0f)
	};

	std::vector<unsigned int>* elements = new std::vector<unsigned int>
	{
		2, 1, 0,
		2, 3, 1
	};



	std::shared_ptr<Mesh> planeMesh = std::make_shared<Mesh>();
	planeMesh->GenerateMesh(positions, nullptr, nullptr, nullptr, elements, 0, GL_STATIC_DRAW, true);
	//configure mesh
	planeMesh->m_meshName = "Plane";
	planeMesh->GetMaterial()->m_defaultDiffuseColor = glm::vec3(0.5f);
	planeMesh->GetMaterial()->m_mapToEnviroment = true;
	std::shared_ptr<ModelNode> modelNode = std::make_shared<ModelNode>(glm::mat4(1.0f));
	modelNode->AddMesh(planeMesh);
	modelNode->SetTransform(glm::scale(glm::mat4(1.0f), glm::vec3(50.0f, 1.0f, 50.0f)));
	
	SetRoot(modelNode);

}

void Plane::Update(const float& dt)
{
}

void Plane::Render(Renderer& renderer, Shader* overideShader)
{
	Shader& shader = overideShader ? *overideShader : ShaderManager.GetShader("main");
	GetRoot()->Render(renderer, shader);
}
