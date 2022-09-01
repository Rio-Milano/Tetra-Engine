#include "ShaderManager.h"
#include<fstream>
#include<iostream>
#include"Shader.h"


ShaderManager& ShaderManager::GetInstance()
{
	static ShaderManager instance;
	return instance;
}



void ShaderManager::AddShader(const std::string& programName, const Shader& shader)
{
	//if shader with name passed already exists then report error
	if (m_programNameProgramIDMap.find(programName) != m_programNameProgramIDMap.end())
	{
		_ASSERT(0);
		std::cout << "Error: Program :" << programName << " already exists.";
		return;
	}
	//else add shader to data store
	m_programNameProgramIDMap[programName] = shader;
}


Shader& ShaderManager::GetShader(const std::string& programName)
{
	//if shader with passed name not in data store then report error
	if (m_programNameProgramIDMap.find(programName) == m_programNameProgramIDMap.end())
	{
		_ASSERT(0);
		std::cout << "Error :" << programName << " does not exist.\n";
	}
	//else return value
	return m_programNameProgramIDMap[programName];
}

void ShaderManager::UpdateAllShaders(const glm::mat4& projection_x_view_mat, glm::vec3 cameraPosition)
{
	//loop all key-value pairs in data store
	for (std::unordered_map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		//get value
		Shader& shader = i->second;
		//update standard uniforms
		shader.SetUniformMat4f(shader.GetLocation("Projection_X_View"), projection_x_view_mat);
		shader.SetUniform3fv(shader.GetLocation("cameraPosition"), cameraPosition);
	}
}

void ShaderManager::DeleteAllShaders()
{
	for (std::unordered_map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		i->second.Delete();
	}
}
