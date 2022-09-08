#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H


#include<unordered_map>
#include<string>
#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include"glmIncludes.h"

class Shader;

class ShaderManager
{
public:
	//singleton
	static ShaderManager& GetInstance();

	//management functions
	void AddShader(const std::string& programName, const Shader& shader);
	Shader& GetShader(const std::string& programName);

	//update projection_view uniforms
	void UpdateAllShaders(const glm::mat4& projection_mat, const glm::mat4& view_mat, glm::vec3 cameraPosition);
	//clean up data store
	void DeleteAllShaders();

private:
	//singleton
	ShaderManager() = default;
	~ShaderManager() = default;

	//data store
	std::unordered_map<std::string, Shader> m_programNameProgramIDMap;
};


#endif