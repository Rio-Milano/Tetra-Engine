#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glad/glad.h>



class Shader
{
public:
	Shader() = default;
	~Shader() = default;

	void Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void Use()const;

	const GLuint& GetID()const;

private:
	GLuint
		m_programID;

	GLuint CreateShader(const std::string& shaderCode, const GLenum& shaderType);
	void LinkShadersIntoProgram(const GLuint& vertexShaderID, const GLuint& fragmentShaderID);
};

#endif