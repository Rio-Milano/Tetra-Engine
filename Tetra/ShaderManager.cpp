#include "ShaderManager.h"
#include<fstream>
#include<iostream>

ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager instance;
	return instance;
}



void ShaderManager::AddShader(const std::string& programName, const Shader& shader)
{
	if (m_programNameProgramIDMap.find(programName) != m_programNameProgramIDMap.end())
	{
		std::cout << "Error: Program :" << programName << " already exists.";
		return;
	}
	m_programNameProgramIDMap[programName] = shader;
}


Shader& ShaderManager::GetShader(const std::string& programName)
{
	if (m_programNameProgramIDMap.find(programName) == m_programNameProgramIDMap.end())
		std::cout << "Error :" << programName << " does not exist.\n";
	
	return m_programNameProgramIDMap[programName];
}

void ShaderManager::UpdateAllShaders(const glm::mat4& projection_x_view_mat)
{
	for (std::map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		Shader& shader = i->second;

		GLuint projection_view_mat_location = glGetUniformLocation(shader.GetID(), "Projection_X_View");

		shader.Use();
		glUniformMatrix4fv(projection_view_mat_location, 1, GL_FALSE, glm::value_ptr(projection_x_view_mat));
	}
}

void ShaderManager::DeleteAllShaders()
{
	for (std::map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		i->second.Delete();
	}
}
