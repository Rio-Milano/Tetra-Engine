#ifndef SHADER_H
#define SHADER_H

#include<string>
#include<glad/glad.h>
#include"glmIncludes.h"


class Shader
{
public:
	//ctors/dtors
	Shader() = default;
	~Shader() = default;

	//takes in paths to shaders to create program
	void Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
	//tell opengl to make the current shader the context
	void Use()const;
	//delete the shader once its no longer needed
	void Delete();

	//utility functions
	void SetUniform3fv(const GLuint& location, const glm::vec3& data);
	void SetUniform1i(const GLuint& location, const int& data);
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

	//internal helpers
	GLuint CreateShader(const std::string& shaderCode, const GLenum& shaderType);
	void LinkShadersIntoProgram(const GLuint& vertexShaderID, const GLuint& fragmentShaderID);
};

#endif