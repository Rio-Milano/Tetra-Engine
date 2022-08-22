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

void ShaderManager::UpdateAllShaders(const glm::mat4& projection_x_view_mat, glm::vec3 cameraPosition)
{
	for (std::map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		Shader& shader = i->second;

		shader.SetUniformMat4f(shader.GetLocation("Projection_X_View"), projection_x_view_mat);
		shader.SetUniform3fv(shader.GetLocation("cameraPosition"), cameraPosition);
	}
}

void ShaderManager::DeleteAllShaders()
{
	for (std::map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		i->second.Delete();
	}
}
