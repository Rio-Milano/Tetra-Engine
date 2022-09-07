#include "Container.h"

Container::Container(const std::string& containerName)
	:Entity(containerName)
{
	if (TextureManager.GetTexture("Box") == nullptr)
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



void Container::Init()
{
	std::vector<glm::vec3> positions
	{
		//back face
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		// front face
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		// left face
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		// right face
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		// bottom face
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		// top face
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, 0.5f, 0.5f), // bottom-left

		/*glm::vec3(-0.5f, -0.5f, -0.5f),
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
		glm::vec3(-0.5f, 0.5f, -0.5f),*/
	};

	std::vector<glm::vec2> textureCords
	{
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),

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
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),

		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f)

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
	mesh->GenerateMesh(positions, normals, textureCords);
	//make material for container
	mesh->GetMaterial()->m_diffuse = TextureManager.GetTexture("Box");
	mesh->GetMaterial()->m_specular = TextureManager.GetTexture("BoxSpec");
	mesh->GetMaterial()->m_emission = TextureManager.GetTexture("BoxEmission");


	std::shared_ptr<ModelNode> modelNode_1 = std::make_shared<ModelNode>(glm::mat4(1.0f), "Box");
	modelNode_1->AddMesh(mesh);


	SetRoot(modelNode_1);

}


void Container::Update(const float& dt)
{
	m_rot += dt;
}

void Container::Render(Renderer& renderer)
{
	for (int i = 0; i < cubePositions.size(); i++)
	{
		GetRoot()->SetTransform(glm::rotate(glm::translate(glm::mat4(1.0f), cubePositions[i] * 2.0f), m_rot, glm::vec3(1.0f)));
		GetRoot()->Render(renderer);
	}
}