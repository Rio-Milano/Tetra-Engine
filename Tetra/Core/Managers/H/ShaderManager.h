#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H


#include<unordered_map>
#include<string>
#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include<memory>

#include"../../Requirements/glmIncludes.h"
#include"../../OpenGL_Abstraction/H/UniformBufferObject.h"

class Shader;

class ShaderManager
{
	friend class LightManager;
public:
	//singleton
	static ShaderManager& GetInstance();

	//management functions
	void AddShader(const std::string& programName, const Shader& shader);
	Shader& GetShader(const std::string& programName);

	void AddUniformBufferObject(const std::string& uboName, const std::shared_ptr<UniformBufferObject>& ubo);
	const std::shared_ptr<UniformBufferObject>& GetUniformBufferObject(const std::string& uboName);


	//clean up data store
	void DeleteAllShaders();

private:
	//singleton
	ShaderManager() = default;
	~ShaderManager() = default;

	//data store
	std::unordered_map<std::string, Shader> m_programNameProgramIDMap;
	std::unordered_map<std::string, std::shared_ptr<UniformBufferObject>> m_uniformBufferObjects;
};


#endif