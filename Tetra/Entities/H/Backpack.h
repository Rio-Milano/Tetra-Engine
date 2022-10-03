#ifndef	BACKPACK_H
#define BACKPACK_H

#include"Entity.h"
#include"../../Core/Graphics/H/Renderer.h"
#include"../../Core/Structure/H/Model.h"
#include<iostream>

#include"../../Core/Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include"../../Core/Managers/H/MeshManager.h"
#define MeshManager MeshManager::GetInstance()

class BackPack : public Entity, public Model
{
public:
	BackPack(const std::string& Name);
	~BackPack() = default;

	void Init()override;

	void Update(const float& dt)override;

	void Render(Renderer& renderer, Shader* overrideShader)override;



};

#endif