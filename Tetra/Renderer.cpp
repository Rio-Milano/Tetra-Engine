#include "Renderer.h"
#include"Entity.h"

#include<iostream>
#include<vector>


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

	glEnable(GL_DEPTH_TEST);

	m_projection = glm::perspective(glm::radians(m_fov), static_cast<float>(winWidth) / static_cast<float>(winHeight), 0.1f, 100.0f);
}

void Renderer::RenderMesh(const Mesh& mesh)
{
	glUseProgram(mesh.m_programID);

	glBindVertexArray(mesh.m_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.m_textureID);

	switch (mesh.m_drawType)
	{
	case 0:
		glDrawElements(GL_TRIANGLES, mesh.m_numberOfElements, GL_UNSIGNED_INT, 0);
		break;
	case 1:
		glDrawArrays(GL_TRIANGLES,  0, mesh.m_triangleCount);
		break;
	default:
		break;
	}


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

void Renderer::SetProjectionViewMatrix(const glm::mat4& viewMatrix, const GLuint& programID)
{
	glUseProgram(programID);

	glm::mat4 prespective_x_view = m_projection * viewMatrix;

	glUniformMatrix4fv(glGetUniformLocation(programID, "Projection_X_View"), 1, GL_FALSE, glm::value_ptr(prespective_x_view));
}

void Renderer::StartRendering()
{
	glClearColor(m_clearColor.x, m_clearColor.y, m_clearColor.z, m_clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
