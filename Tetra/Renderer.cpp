#include "Renderer.h"
#include"Entity.h"
#include<iostream>
#include<vector>

void Renderer::CreateWindow(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//create window
	m_window.CreateWindow(renderWindowWidth, renderWindowHeight, renderWindowName);
}

void Renderer::UpdateViewportWhenWindowSizeChanged(GLFWwindow* window, int windowWidth, int windowHeight)
{
	glViewport(0, 0, windowWidth, windowHeight);
}

void Renderer::InitRenderer()
{
	int winWidth, winHeight;
	glfwGetWindowSize(m_window.GetWindowPtr(), &winWidth, &winHeight);
	glViewport(0, 0, winWidth, winHeight);

	glfwSetFramebufferSizeCallback(m_window.GetWindowPtr(), UpdateViewportWhenWindowSizeChanged);


	glEnable(GL_DEPTH_TEST);
}

void Renderer::RenderMesh(const Mesh& mesh)
{
	glUseProgram(mesh.GetProgramID());

	glBindVertexArray(mesh.GetVAO());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh.GetTextureID());

	switch (mesh.GetDrawType())
	{
	case 0:
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.GetNumberOfElements()), GL_UNSIGNED_INT, 0);
		break;
	case 1:
		glDrawArrays(GL_TRIANGLES,  0, mesh.GetVertexCount());
		break;
	default:
		break;
	}


	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
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
	Shader shader;
	shader.Create("Data/Shaders/vertexShader.vert", "Data/Shaders/fragmentShader.frag");
	ShaderManager.AddShader("main", shader);
}
