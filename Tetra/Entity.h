#ifndef ENTITY_H
#define ENTITY_H

#include"glmIncludes.h"
#include<glad/glad.h>

#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()


class Renderer;

class Entity
{
public:
	virtual void Init() = 0;

	virtual void Update(const float& dt) = 0;
	virtual void Render(Renderer& renderer) = 0;
};


#endif