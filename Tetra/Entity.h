#ifndef ENTITY_H
#define ENTITY_H

#include"glmIncludes.h"
#include<glad/glad.h>

#include"Program_Shader_Managment.h"
#define Program_Shader_Managment Program_Shader_Managment::GetInstance()

struct Mesh
{
	~Mesh()
	{
		//delete resources like buffers/how to do??
		glDeleteVertexArrays(1, &m_VAO);
	}
	GLuint m_VAO;
	GLuint m_numberOfElements;
	GLuint m_textureID;
	GLuint m_programID;
	GLuint m_drawType;
	GLuint m_triangleCount;
};

struct Transform
{
	glm::mat4 m_transform{ 1.0f };
};

class Renderer;

class Entity
{
public:
	virtual void Update() = 0;
	virtual void Render(Renderer& renderer) = 0;
};


#endif