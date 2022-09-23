#ifndef AQUAPIG_H
#define AQUAPIG_H

#include"Entity.h"
#include"Renderer.h"
#include"Model.h"
#include<iostream>

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include"MeshManager.h"
#define MeshManager MeshManager::GetInstance()

class AquaPig : public Entity, public Model
{
public:
	AquaPig(const std::string& aquaPigName);
	~AquaPig() = default;

	void Init();

	void Update(const float& dt);

	void Render(Renderer& renderer);



};

#endif