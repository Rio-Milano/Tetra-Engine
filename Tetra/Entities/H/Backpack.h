#ifndef	BACKPACK_H
#define BACKPACK_H

#include"Entity.h"
#include"Renderer.h"
#include"Model.h"
#include<iostream>

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include"MeshManager.h"
#define MeshManager MeshManager::GetInstance()

class BackPack : public Entity, public Model
{
public:
	BackPack(const std::string& Name);
	~BackPack() = default;

	void Init();

	void Update(const float& dt);

	void Render(Renderer& renderer);



};

#endif