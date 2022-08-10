#include "Program_Shader_Managment.h"
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


const Shader& ShaderManager::GetShader(const std::string& programName)
{
	if (m_programNameProgramIDMap.find(programName) == m_programNameProgramIDMap.end())
		std::cout << "Error :" << programName << " does not exist.\n";
	
	return m_programNameProgramIDMap[programName];
}
