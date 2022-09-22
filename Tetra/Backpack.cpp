#include "BackPack.h"
BackPack::BackPack(const std::string& Name)
	:Entity(Name)
{

};

void BackPack::Init()
{
	std::shared_ptr<Model> backPack = std::make_shared<Model>("Data/Models/backpack/backpack.obj");//back pack is an optional extension


	_ASSERT(backPack != nullptr);

	//take the nodes from each model so we can build hirearchy
	std::shared_ptr<ModelNode>
		nbackPack = backPack->GetRoot();

	

	glm::mat4 TbackPack(1.0f);
	TbackPack = glm::translate(TbackPack, glm::vec3(-10.f, 0.f, -10.0f));
	TbackPack = glm::rotate(TbackPack, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	TbackPack = glm::scale(TbackPack, glm::vec3(0.4f));
	nbackPack->SetAssimpNodeTransform(TbackPack);

	
	for (size_t i = 0; i < nbackPack->GetChildren().size(); i++)
	{
		nbackPack->GetChildren()[i]->GetMeshes()[0]->SetProgramName("explode");
	}

	SetRoot(nbackPack);
}

void BackPack::Update(const float& dt)
{
}

void BackPack::Render(Renderer& renderer)
{
	static_cast<Model*>(this)->Render(renderer);
}