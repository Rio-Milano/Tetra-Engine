#ifndef AQUAPIG_H
#define AQUAPIG_H

#include"Entity.h"
#include"../../Core/Graphics/H/Renderer.h"
#include"../../Core/Structure/H/Model.h"
#include<iostream>

#include"../../Core/Managers/H/TextureManager.h"
#define TextureManager TextureManager::GetInstance()

#include"../../Core/Managers/H/MeshManager.h"
#define MeshManager MeshManager::GetInstance()

class AquaPig : public Entity, public Model
{
public:
	AquaPig(const std::string& aquaPigName);
	~AquaPig() = default;

	void Init() override;

	void Update(const float& dt) override;

	void Render(Renderer& renderer, Shader* overrideShader) override;



};

#endif