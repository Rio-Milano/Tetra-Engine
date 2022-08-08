#include "Renderer.h"
#include"Entity.h"

#include<iostream>
#include<vector>


void Renderer::LoadMeshes()
{


	std::cout << "\n\n\n\n\n\n";
	Helper_::Status_::DisplayUsefulInfo();
}

void Renderer::CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//create window
	m_window.CreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName);
}

void Renderer::InitRenderer()
{
	int winWidth, winHeight;
	glfwGetWindowSize(m_window.GetWindowPtr(), &winWidth, &winHeight);

	glViewport(0, 0, winWidth, winHeight);

	CreateShaders();
}

void Renderer::RenderMesh(const Mesh& mesh)
{
	glBindVertexArray(mesh.m_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.m_textureID);

	glUseProgram(mesh.m_programID);

	glDrawElements(GL_TRIANGLES, mesh.m_numberOfElements, GL_UNSIGNED_INT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Renderer::StartRendering()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::EndRendering()
{
	glfwSwapBuffers(m_window.GetWindowPtr());
}







Window& Renderer::GetWindow()
{
	return m_window;
}

void Renderer::SetClearColor(const glm::vec4& clearColor)
{
	m_clearColor = clearColor;
}

void Renderer::CreateShaders()
{
	//Create Shaders
	const GLuint vertexShaderID{ Helper_::Shaders_::CreateShader(GL_VERTEX_SHADER, "Data/Shaders/vertexShader.vert") };
	const GLuint fragmentShaderID{ Helper_::Shaders_::CreateShader(GL_FRAGMENT_SHADER, "Data/Shaders/fragmentShader.frag") };
	const GLuint programID{ Helper_::Shaders_::CreateProgram(vertexShaderID, fragmentShaderID) };
	Program_Shader_Managment.AddProgram("main", programID);
}
