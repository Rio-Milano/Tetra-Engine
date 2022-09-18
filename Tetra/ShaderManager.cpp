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
	_ASSERT(m_programNameProgramIDMap.find(programName) == m_programNameProgramIDMap.end());


	const GLuint& shaderObject = shader.GetID();

	for (std::unordered_map<std::string, std::shared_ptr<UniformBufferObject>>::iterator i = m_uniformBufferObjects.begin(); i != m_uniformBufferObjects.end(); i++)
	{
		const std::shared_ptr<UniformBufferObject>& ubo = i->second;
		//get the name of the uniform block from the ubo
		const std::string& uniformBlockName = ubo->m_uniformBlockName;

		//get the uniform block from the shader
		GLuint uniformBlockIndex =  glGetUniformBlockIndex(shaderObject, uniformBlockName.c_str());

		//set the binding point of the uniform block in the shader
		glUniformBlockBinding(shaderObject, uniformBlockIndex, ubo->m_bindingPoint);
	}

	//else add shader to data store
	m_programNameProgramIDMap[programName] = shader;
}

void ShaderManager::AddUniformBufferObject(const std::string& uboName, const std::shared_ptr<UniformBufferObject>& ubo)
{
	//if already ubo in storage break point
	_ASSERT(m_uniformBufferObjects.find(uboName) == m_uniformBufferObjects.end());

	m_uniformBufferObjects[uboName] = ubo;
}

const std::shared_ptr<UniformBufferObject>& ShaderManager::GetUniformBufferObject(const std::string& uboName)
{
	//if name not fonnd
	_ASSERT(m_uniformBufferObjects.find(uboName) != m_uniformBufferObjects.end());

	return m_uniformBufferObjects[uboName];
}

Shader& ShaderManager::GetShader(const std::string& programName)
{
	//if shader with passed name not in data store then report error
	_ASSERT(m_programNameProgramIDMap.find(programName) != m_programNameProgramIDMap.end());
	
	//else return value
	return m_programNameProgramIDMap[programName];
}

void ShaderManager::DeleteAllShaders()
{
	for (std::unordered_map<std::string, Shader>::iterator i = m_programNameProgramIDMap.begin(); i != m_programNameProgramIDMap.end(); i++)
	{
		i->second.Delete();
	}
}
