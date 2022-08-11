#include "Application.h"

#include<iostream>

void Application::PushLayer(const std::shared_ptr<BaseLayer>& newLayer)
{
	m_baseLayer = newLayer;
}

void Application::Run()
{

	if (m_baseLayer)
	{
		m_baseLayer->CreateLayer(glm::vec<2, int>(800, 600), "Tetra");
		m_baseLayer->Start();

		float elapsedTime = 0.f;
		
		while (!glfwWindowShouldClose(m_baseLayer->m_renderer.GetWindow().GetWindowPtr()))
		{
			float 
				glTime = static_cast<float>(glfwGetTime()),
				dt = glTime - elapsedTime;
			elapsedTime = glTime;


			m_baseLayer->BaseUpdate(dt);

			m_baseLayer->m_renderer.StartRendering();
			m_baseLayer->Render();
			m_baseLayer->m_renderer.EndRendering();
		}
		m_baseLayer->End();
		m_baseLayer->DestroyLayer();

	}
	else
	{
		std::cout << "No base layer found!\n";
	}
}

