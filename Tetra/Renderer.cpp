#include "Renderer.h"
#include"Entity.h"
#include<iostream>
#include<vector>

#include"ShaderManager.h"
#define ShaderManager ShaderManager::GetInstance()
#include"Texture.h"

void Renderer::CreateWindowGLFW(const int& renderWindowWidth, const int& renderWindowHeight, const std::string& renderWindowName)
{
	//create window
	m_window.CreateWindowGLFW(renderWindowWidth, renderWindowHeight, renderWindowName);
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

void Renderer::RenderMesh(const Mesh& mesh, const glm::mat4& worldMat)const
{
	//set uniforms
	Shader& shader = ShaderManager.GetShader(mesh.m_programName);
	
	shader.SetUniformMat4f(shader.GetLocation("worldMat"), worldMat);
	
	glm::mat3 normalMat = glm::mat3(glm::transpose(glm::inverse(worldMat))); 
	shader.SetUniformMat3f(shader.GetLocation("normalMat"), normalMat);

	//get ready to render
	shader.Use();

	shader.SetUniform3fv(shader.GetLocation("material.defaultDiffuseColor"), mesh.m_defaultDiffuseColor);
	shader.SetUniform3fv(shader.GetLocation("material.defaultSpecularColor"), mesh.m_defaultSpecularColor);
	shader.SetUniform1f(shader.GetLocation("material.ambientIntensity"), mesh.m_ambientIntensity);
	shader.SetUniform1f(shader.GetLocation("material.specularIntensity"), mesh.m_specularIntensity);
	shader.SetUniform1f(shader.GetLocation("material.emissionRange"), mesh.emissionRange);

	const std::shared_ptr<Texture>& diffuseTexture = mesh.m_diffuse;
	if (diffuseTexture != nullptr && diffuseTexture->GetTextureAttributes().validTexture)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture->GetTextureAttributes().textureID);
		shader.SetUniform1i(shader.GetLocation("material.diffuseMap"), 0);
		shader.SetUniform1b(shader.GetLocation("material.hasDiffuseMap"), true);

	}
	else
	{
		shader.SetUniform1b(shader.GetLocation("material.hasDiffuseMap"), false);
	}

	const std::shared_ptr<Texture>& specularTexture = mesh.m_specular;
	if (specularTexture != nullptr && specularTexture->GetTextureAttributes().validTexture)
	{
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture->GetTextureAttributes().textureID);
		shader.SetUniform1i(shader.GetLocation("material.specularMap"), 1);
		shader.SetUniform1b(shader.GetLocation("material.hasSpecularMap"), true);
	}
	else
	{
		shader.SetUniform1b(shader.GetLocation("material.hasSpecularMap"), false);
	}

	const std::shared_ptr<Texture>& emissionTexture = mesh.m_emission;
	if (emissionTexture != nullptr && emissionTexture->GetTextureAttributes().validTexture)
	{
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, emissionTexture->GetTextureAttributes().textureID);
		shader.SetUniform1i(shader.GetLocation("material.emissionMap"), 2);
		shader.SetUniform1b(shader.GetLocation("material.hasEmissionMap"), true);
	}
	else
	{
		shader.SetUniform1b(shader.GetLocation("material.hasEmissionMap"), false);
	}
	
	glBindVertexArray(mesh.m_VAO);

	switch (mesh.m_drawType)
	{
	case 0:
		glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.m_elements.size()), GL_UNSIGNED_INT, 0);
		break;
	case 1:
		glDrawArrays(GL_TRIANGLES,  0, static_cast<GLsizei>(mesh.m_verticies.size()));
		break;
	default:
		break;
	}

	glActiveTexture(GL_TEXTURE0);
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
