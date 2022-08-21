#include "Shader.h"


#include<fstream>
#include<iostream>
void LoadStringFromFile(const std::string& fileLocation, std::string& data)
{
	//try to open file
	std::fstream fileStream(fileLocation, std::fstream::in);
	
	//if file not open
	if (!fileStream.is_open())
	{
		//display error
		std::cout << std::string("Shader file at :" + fileLocation + " could not be opened.\n");
		//exit
		return;
	}

	//initialize a string with stream data
	std::string fileContent((std::istreambuf_iterator<char>(fileStream)), (std::istreambuf_iterator<char>()));//string from file
	
	//coppy over string data
	data = fileContent;

	//close file
	fileStream.close();
};


void Shader::Create(const std::string& vertexShaderPath, const std::string& fragmentShaderPath)
{
	std::string vertexShaderSource;
	LoadStringFromFile(vertexShaderPath, vertexShaderSource);
	GLuint vertexShaderID = CreateShader(vertexShaderSource, GL_VERTEX_SHADER);

	std::string fragmentShaderSource;
	LoadStringFromFile(fragmentShaderPath, fragmentShaderSource);
	GLuint fragmentShaderID = CreateShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

	LinkShadersIntoProgram(vertexShaderID, fragmentShaderID);
}

void Shader::Use() const
{
	glUseProgram(m_programID);
}

void Shader::Delete()
{
	glDeleteShader(m_programID);
}

void Shader::SetUniform3fv(const GLuint& location, float* data)
{
	Use();
	glUniform3fv(location, 1, data);
}

void Shader::SetUniform1f(const GLuint& location, const float& data)
{
	Use();
	glUniform1f(location, data);
}

void Shader::SetUniform1ui(const GLuint& location, const unsigned int& data)
{
	Use();
	glUniform1ui(location, data);
}

void Shader::SetUniform1b(const GLuint& location, const bool& data)
{
	Use();
	glUniform1i(location, static_cast<int>(data));
}

void Shader::SetUniformMat4f(const GLuint location, const glm::mat4& mat4)
{
	Use();
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::SetUniformMat3f(const GLuint location, const glm::mat3& mat3)
{
	Use();
	glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat3));
}

GLuint Shader::GetLocation(const std::string& string) const
{
	return glGetUniformLocation(m_programID, string.c_str());
}

const GLuint& Shader::GetID() const
{
	return m_programID;
}

GLuint Shader::CreateShader(const std::string& shaderCode, const GLenum& shaderType)
{
	//create shader
	GLuint shaderID = glCreateShader(shaderType);
	
	//get char pointer to shader source
	const char* shaderSrcCharPtr = shaderCode.c_str();

	//load source code
	glShaderSource(shaderID, 1, &shaderSrcCharPtr, 0);

	//compile the shader
	glCompileShader(shaderID);

	//flag of compilation success
	int success;

	//buffer for compilation errors
	char errorMessages[512];

	//get success of compilation
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);

	//get string of shader type
	std::string shaderTypeName{ shaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader" };

	//if compilation failed
	if (!success)
	{
		//add errors of compilation to error buffer
		glGetShaderInfoLog(shaderID, 512, 0, errorMessages);

		//output errors
		std::cout << std::string("Error Compiling " + shaderTypeName + ", error :" + errorMessages + "\n");
		
		//exit
		return 0;
	}

	//output success
	std::cout << shaderTypeName << " Created!\n";

	return shaderID;
}

void Shader::LinkShadersIntoProgram(const GLuint& vertexShaderID, const GLuint& fragmentShaderID)
{
	//create program
	m_programID = glCreateProgram();

	//attach shaders to program
	glAttachShader(m_programID, vertexShaderID);
	glAttachShader(m_programID, fragmentShaderID);

	//link the program
	glLinkProgram(m_programID);

	//delete shaders as no longer needed
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	//link success flag
	int linkStatus;

	//link error buffer
	char linkErrors[512];

	//get link status
	glGetProgramiv(m_programID, GL_LINK_STATUS, &linkStatus);
	
	//if link failed
	if (!linkStatus)
	{
		//get errors
		glGetProgramInfoLog(m_programID, 512, 0, linkErrors);

		std::cout << "Program failed to link shaders, error :" << linkErrors << std::endl;
		return;
	}

	std::cout << "Program Created!\n";

}
