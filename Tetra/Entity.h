#ifndef ENTITY_H
#define ENTITY_H

#include"glmIncludes.h"
#include<glad/glad.h>

#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()


struct Transform
{
	glm::mat4 m_transform{ 1.0f };
};

class Renderer;

class Entity
{
public:
	virtual void Init() = 0;

	virtual void Update() = 0;
	virtual void Render(Renderer& renderer) = 0;
};


#endif