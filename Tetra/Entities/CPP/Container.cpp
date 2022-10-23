#include "../H/Container.h"

Container::Container(const std::string& containerName)
	:Entity(containerName)
{
	//if (TextureManager.GetTexture("Box") == nullptr)
	//{
	//	std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>();
	//	diffuseTexture->InitializeTexture("Data/Textures/Box.png", false, true);
	//	TextureManager.AddTexture("Box", diffuseTexture);
	//}

	//if (TextureManager.GetTexture("BoxSpec") == nullptr)
	//{
	//	std::shared_ptr<Texture> specularTexture = std::make_shared<Texture>();
	//	specularTexture->InitializeTexture("Data/Textures/BoxSpec.png");
	//	TextureManager.AddTexture("BoxSpec", specularTexture);
	//}

	//if (TextureManager.GetTexture("BoxEmission") == nullptr)
	//{
	//	std::shared_ptr<Texture> emissionTexture = std::make_shared<Texture>();
	//	emissionTexture->InitializeTexture("Data/Textures/BoxEmission.png");
	//	TextureManager.AddTexture("BoxEmission", emissionTexture);
	//}

	if (TextureManager.GetTexture("Brick") == nullptr)
	{
		std::shared_ptr<Texture> diffuseTexture = std::make_shared<Texture>();
		diffuseTexture->InitializeTexture("Data/Textures/Brick.jpg", true, true);
		TextureManager.AddTexture("Brick", diffuseTexture);
	}


	if (TextureManager.GetTexture("BrickNormal") == nullptr)
	{
		std::shared_ptr<Texture> normalTexture = std::make_shared<Texture>();
		normalTexture->InitializeTexture("Data/Textures/BrickNormal.jpg", true);
		TextureManager.AddTexture("BrickNormal", normalTexture);
	}

	if (TextureManager.GetTexture("BrickDisplacement") == nullptr)
	{
		std::shared_ptr<Texture> displacementTexture = std::make_shared<Texture>();
		displacementTexture->InitializeTexture("Data/Textures/BrickDisplacement.jpg",true);
		TextureManager.AddTexture("BrickDisplacement", displacementTexture);
	}



};



void Container::Init()
{
	std::vector<glm::vec3>* positionsPtr = new std::vector<glm::vec3>
	{
		//back face
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right

		
		//// front face
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-left

		//// left face
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, 0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, -0.5f, -0.5f), // bottom-left
		//// right face
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, -0.5f, -0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, 0.5f, 0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left

		//// bottom face
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left
		glm::vec3(-0.5f, -0.5f, 0.5f), // bottom-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		glm::vec3(-0.5f, -0.5f, -0.5f), // top-right
		glm::vec3(0.5f, -0.5f, -0.5f), // top-left
		glm::vec3(0.5f, -0.5f, 0.5f), // bottom-left

		//// top face
		glm::vec3(-0.5f, 0.5f, 0.5f), // bottom-left
		glm::vec3(0.5f, 0.5f, 0.5f), // bottom-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(0.5f, 0.5f, -0.5f), // top-right
		glm::vec3(-0.5f, 0.5f, -0.5f), // top-left
		glm::vec3(-0.5f, 0.5f, 0.5f), // bottom-left
	};

	std::vector<glm::vec2>* texCordsPtr = new std::vector<glm::vec2>
	{
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 0.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),

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

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
		glm::vec2(0.0f, 0.0f),
		
	};


	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();
	mesh->GenerateMesh(positionsPtr, nullptr, texCordsPtr, nullptr, nullptr, 1, GL_STATIC_DRAW, true, true);
	
	//make material for container
	mesh->GetMaterial()->m_diffuse = TextureManager.GetTexture("Brick");
	//mesh->GetMaterial()->m_specular = TextureManager.GetTexture("BoxSpec");
	mesh->GetMaterial()->m_normal = TextureManager.GetTexture("BrickNormal");
	mesh->GetMaterial()->m_displacement = TextureManager.GetTexture("BrickDisplacement");
	//mesh->GetMaterial()->m_emission = TextureManager.GetTexture("BoxEmission");
	
	/*mesh->GetMaterial()->m_mapToEnviroment = true;
	mesh->GetMaterial()->m_toRefractiveIndex = RefractiveIndex::Glass;
	mesh->GetMaterial()->m_reflectionType = ReflectionType::Reflection;*/

	//const unsigned int numOfInstances = 20;

	//std::vector<glm::mat4> instancedTransforms;
	//instancedTransforms.resize(numOfInstances);

	//for (size_t i = 0; i < numOfInstances; i++)
	//{
	//	instancedTransforms[i] = glm::translate(glm::mat4(1), glm::vec3(i, 0, i));
	//}

	//mesh->CreateInstances(&instancedTransforms);

	std::shared_ptr<ModelNode> modelNode_1 = std::make_shared<ModelNode>(glm::mat4(1.0f));
	modelNode_1->AddMesh(mesh);

	SetRoot(modelNode_1);

}


void Container::Update(const float& dt)
{
	m_rot += 0.5;
	if (m_rot > 360.0f)
		m_rot = 0.0f;
}

void Container::Render(Renderer& renderer, Shader* overideShader)
{
	Shader& shader = overideShader ? *overideShader : ShaderManager.GetShader("main");
	float angle = glm::radians(m_rot);
	//angle = 0.0f;
	for (int i = 0; i < cubePositions.size(); i++)
	{
		GetRoot()->SetTransform(glm::rotate(glm::translate(glm::mat4(1.0f), cubePositions[i] * 2.0f), angle, glm::vec3(1.0f)));
		GetRoot()->Render(renderer, shader);
	}


}