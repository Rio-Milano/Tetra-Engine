#include "SandBoxLayer.h"

void SandBoxLayer::Start()
{

	m_entity.Init();
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
	m_entity.Render(m_renderer);
}

void SandBoxLayer::End()
{
}
