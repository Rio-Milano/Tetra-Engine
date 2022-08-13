#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include"Shader.h"

#include<map>
#include<string>
#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>
#include"glmIncludes.h"


class ShaderManager
{
public:

	static ShaderManager& GetInstance();

	void AddShader(const std::string& programName, const Shader& shader);
	Shader& GetShader(const std::string& programName);

	void UpdateAllShaders(const glm::mat4& projection_x_view_mat);
	void DeleteAllShaders();

private:
	ShaderManager() = default;
	~ShaderManager() = default;

	std::map<std::string, Shader> m_programNameProgramIDMap;
};


#endif