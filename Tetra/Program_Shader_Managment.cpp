#include "Program_Shader_Managment.h"
#include<fstream>
#include<iostream>

Program_Shader_Managment& Program_Shader_Managment::GetInstance()
{
	static Program_Shader_Managment instance;
	return instance;
}



void Program_Shader_Managment::AddProgram(const std::string& programName, const GLuint& programID)
{
	if (m_programNameProgramIDMap.find(programName) != m_programNameProgramIDMap.end())
	{
		std::cout << "Error: Program :" << programName << " already exists.";
		return;
	}
	m_programNameProgramIDMap[programName] = programID;
}

const GLuint& Program_Shader_Managment::GetProgramID(const std::string& programName)
{
	if (m_programNameProgramIDMap.find(programName) == m_programNameProgramIDMap.end())
	{
		std::cout << "Error :" << programName << " does not exist.\n";
		return 0;
	}
	return m_programNameProgramIDMap[programName];
}
