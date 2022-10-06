#include "../H/AquaPig.h"

AquaPig::AquaPig(const std::string& aquaPigName)
	:Entity(aquaPigName)
{
	if (TextureManager.GetTexture("AquaPigDiffuse") == nullptr)
	{
		std::shared_ptr<Texture> aquaPigTexture = std::make_shared<Texture>();
		aquaPigTexture->InitializeTexture("Data/Models/AquaPig/aqua_pig_1K.jpg", false, true);
		TextureManager.AddTexture("AquaPigDiffuse", aquaPigTexture);
	}

	if (TextureManager.GetTexture("AquaPigSpecular") == nullptr)
	{
		std::shared_ptr<Texture> aquaPigTexture = std::make_shared<Texture>();
		aquaPigTexture->InitializeTexture("Data/Models/AquaPig/specular.jpg");
		TextureManager.AddTexture("AquaPigSpecular", aquaPigTexture);
	}


};

void AquaPig::Init()
{

	//load in the seperate .obj files
	std::shared_ptr<Model>
		gun = std::make_shared<Model>("Data/Models/AquaPig/gun.obj"),
		gun_base = std::make_shared<Model>("Data/Models/AquaPig/gun_base.obj"),
		hull = std::make_shared<Model>("Data/Models/AquaPig/hull.obj"),
		propeller = std::make_shared<Model>("Data/Models/AquaPig/propeller.obj"),
		wing_left = std::make_shared<Model>("Data/Models/AquaPig/wing_left.obj"),
		wing_right = std::make_shared<Model>("Data/Models/AquaPig/wing_right.obj");
	

	//make sure all models loaded
	if (!(gun && gun_base && hull && propeller && wing_left && wing_left))
		_ASSERT(false);

	//take the nodes from each model so we can build hirearchy
	std::shared_ptr<ModelNode>
		ngun = gun->GetRoot(),
		ngun_base = gun_base->GetRoot(),
		nhull = hull->GetRoot(),
		npropeller = propeller->GetRoot(),
		nwing_left = wing_left->GetRoot(),
		nwing_right = wing_right->GetRoot();

	//construct hirearchy
	nhull->AddChildren({ nwing_left, nwing_right, npropeller, ngun_base });
	ngun_base->AddChild(ngun);

	//construct transform hirearchy
	nhull->SetAssimpNodeTransform(glm::translate(nhull->GetAssimpNodeTransform(), glm::vec3(8, 0, 8)));

	glm::mat4 Twing_right(1.0f);
	Twing_right = glm::translate(Twing_right, glm::vec3(-2.231, 0.272, -2.663));
	nwing_right->SetAssimpNodeTransform(Twing_right);

	glm::mat4 Twing_left(1.0f);
	Twing_left = glm::translate(Twing_left, glm::vec3(2.231, 0.272, -2.663));
	nwing_left->SetAssimpNodeTransform(Twing_left);

	glm::mat4 Tpropeller(1.0f);
	Tpropeller = glm::translate(Tpropeller, glm::vec3(0, 1.395, -3.616));
	Tpropeller = glm::rotate(Tpropeller, (float)glm::radians(90.0f), glm::vec3(1, 0, 0));
	npropeller->SetAssimpNodeTransform(Tpropeller);

	glm::mat4 Tgun_base(1.0f);
	Tgun_base = glm::translate(Tgun_base, glm::vec3(0, 0.569, -1.866));
	ngun_base->SetAssimpNodeTransform(Tgun_base);

	glm::mat4 Tgun(1.0f);
	Tgun = glm::translate(Tgun, glm::vec3(0, 1.506, 0.644));
	ngun->SetAssimpNodeTransform(Tgun);


	std::shared_ptr<Texture> aquaPigDiffuse = TextureManager.GetTexture("AquaPigDiffuse");


	//assign the diffuse texture to each mesh
	ngun->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_diffuse = aquaPigDiffuse;
	ngun_base->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_diffuse = aquaPigDiffuse;
	nhull->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_diffuse = aquaPigDiffuse;
	npropeller->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_diffuse = aquaPigDiffuse;
	nwing_left->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_diffuse = aquaPigDiffuse;
	nwing_right->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_diffuse = aquaPigDiffuse;

	std::shared_ptr<Texture> aquaPigSpecular = TextureManager.GetTexture("AquaPigSpecular");

	//assign the diffuse texture to each mesh
	ngun->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_specular = aquaPigSpecular;
	ngun_base->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_specular = aquaPigSpecular;
	nhull->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_specular = aquaPigSpecular;
	npropeller->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_specular = aquaPigSpecular;
	nwing_left->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_specular = aquaPigSpecular;
	nwing_right->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_specular = aquaPigSpecular;


	//set each node to have an outline
	ngun->GetChildren()[0]->SetDrawOutline(1);
	ngun_base->GetChildren()[0]->SetDrawOutline(1);
	nhull->GetChildren()[0]->SetDrawOutline(1);
	npropeller->GetChildren()[0]->SetDrawOutline(1);
	nwing_left->GetChildren()[0]->SetDrawOutline(1);
	nwing_right->GetChildren()[0]->SetDrawOutline(1);
	nhull->GetChildren()[0]->SetDrawOutline(1);

	nhull->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;
	ngun->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;
	ngun_base->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;
	nhull->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;
	npropeller->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;
	nwing_left->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;
	nwing_right->GetChildren()[0]->GetMeshes()[0]->GetMaterial()->m_mapToEnviroment = true;;
	
	nhull->SetTransform(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, 0.0f)));

	SetRoot(nhull);

	//const unsigned int numOfInstances = 20;

	//std::vector<glm::mat4> instancedTransforms;
	//instancedTransforms.resize(numOfInstances);

	//for (size_t i = 0; i < numOfInstances; i++)
	//{
	//	instancedTransforms[i] = glm::translate(glm::mat4(1), glm::vec3(i, 0, i));
	//}

	//this->CreateInstances(&instancedTransforms);
}

void AquaPig::Update(const float& dt)
{
	std::shared_ptr<ModelNode> propellerNode = FindNode("propeller.obj");
	propellerNode->SetTransform(glm::rotate(propellerNode->GetTransform(), (float)glm::radians(400.0f * dt), glm::vec3(0, 1, 0)));

	std::shared_ptr<ModelNode> hullNode = FindNode("hull.obj");
	hullNode->SetTransform(glm::rotate(hullNode->GetTransform(), (float)glm::radians(sin(glfwGetTime())) * dt * 10.0f, glm::vec3(0, 1, 0)));

	std::shared_ptr<ModelNode> gunNode = FindNode("gun");
	gunNode->SetTransform(glm::rotate(gunNode->GetTransform(), (float)glm::radians(sin(glfwGetTime())) * dt * 20.0f, glm::vec3(1, 0, 0)));

}

void AquaPig::Render(Renderer& renderer, Shader* overrideShader)
{
	Shader& shader = overrideShader ? *overrideShader :  ShaderManager.GetShader("main");

	static_cast<Model*>(this)->Render(renderer, shader);
}


