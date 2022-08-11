#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include"Shader.h"

#include<map>
#include<string>
#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>


class ShaderManager
{
public:

	static ShaderManager& GetInstance();

	void AddShader(const std::string& programName, const Shader& shader);
	const Shader& GetShader(const std::string& programName);

private:
	ShaderManager() = default;
	~ShaderManager() = default;

	std::map<std::string, Shader> m_programNameProgramIDMap;
};


#endif