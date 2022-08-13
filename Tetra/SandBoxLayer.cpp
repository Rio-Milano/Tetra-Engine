#include "SandBoxLayer.h"

#include"TextureManager.h"
#define TextureManager TextureManager::GetInstance()

void SandBoxLayer::Start()
{
	Texture bricks;
	bricks.InitializeTexture("Data/Images/Container.jpg");
	TextureManager.AddTexture("Bricks", bricks);

	m_entity.Init();
	
	m_lightManager.Initialize();

	m_lightManager.SetShaderID(&ShaderManager.GetShader("main"));
	m_lightManager.SetAmbientLight(glm::vec3(0.87f, 0.5075f, 0.f), .1f);

	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, 8.f), glm::vec3(.0f, 1.f, 0.f), .6f);
	m_lightManager.SetPointLight(glm::vec3(0.f, 0.f, -8.f), glm::vec3(0.0f, 1.f, 0.f), .6f); 
	m_lightManager.SetPointLight(glm::vec3(-8.f, 0.f, 0.f), glm::vec3(0.0f, 0.f, 1.f), .6f);
	m_lightManager.SetPointLight(glm::vec3(8.f, 0.f, 0.f), glm::vec3(0.0f, 0.f, 1.f), .6f);

	m_lightManager.SetDirectionalLight(glm::vec3(-1.f, -1.f, 0.f), glm::vec3(1.0f), .3f);
}

void SandBoxLayer::Update(float dt)
{
	if (glfwGetKey(m_renderer.GetWindow().GetWindowPtr(), GLFW_KEY_0) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (glfwGetKey(m_renderer.GetWindow().GetWindowPtr(), GLFW_KEY_9) == GLFW_PRESS)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	m_entity.Update();
}

void SandBoxLayer::Render()
{
	m_lightManager.UpdateShader();

	m_lightManager.DrawLights(m_renderer);
	m_entity.Render(m_renderer);
}

void SandBoxLayer::End()
{
}
