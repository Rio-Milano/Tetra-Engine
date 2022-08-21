#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glad/glad.h>
#include"glmIncludes.h"


class Shader
{
public:
	Shader() = default;
	~Shader() = default;

	void Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	void Use()const;
	void Delete();

	void SetUniform3fv(const GLuint& location, float* data);
	void SetUniform1f(const GLuint& location, const float& data);
	void SetUniform1ui(const GLuint& location, const unsigned int& data);
	void SetUniform1b(const GLuint& location, const bool& data);
	void SetUniformMat4f(const GLuint location, const glm::mat4& mat4);
	void SetUniformMat3f(const GLuint location, const glm::mat3& mat3);

	GLuint GetLocation(const std::string& string) const;

	const GLuint& GetID()const;

private:
	GLuint
		m_programID;

	GLuint CreateShader(const std::string& shaderCode, const GLenum& shaderType);
	void LinkShadersIntoProgram(const GLuint& vertexShaderID, const GLuint& fragmentShaderID);
};

#endif