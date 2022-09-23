#ifndef ENTITY_H
#define ENTITY_H

#include<glad/glad.h>
#include<string>

#include"../../Requirements/glmIncludes.h"
#include"../../Core/Managers/H/ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()


class Renderer;

class Entity
{
public:
	Entity(const std::string& entityName) :m_entityName(entityName) {};

	virtual void Init() = 0;

	virtual void Update(const float& dt) = 0;
	virtual void Render(Renderer& renderer) = 0;

private:
	std::string m_entityName{ "Default" };
};


#endif