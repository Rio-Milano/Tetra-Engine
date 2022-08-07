#ifndef PROGRAM_SHADER_MANAGMENT_H
#define PROGRAM_SHADER_MANAGMENT_H

#include<map>
#include<string>
#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>

class Program_Shader_Managment
{
public:

	static Program_Shader_Managment& GetInstance();

	void AddProgram(const std::string& programName, const GLuint& programID);
	const GLuint& GetProgramID(const std::string& programName);

private:
	Program_Shader_Managment() = default;
	~Program_Shader_Managment() = default;

	std::map<std::string, GLuint> m_programNameProgramIDMap;
};

#endif