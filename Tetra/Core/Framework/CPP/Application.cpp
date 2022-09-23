#include "../H/Application.h"

#include<iostream>

void Application::PushLayer(const std::shared_ptr<BaseLayer>& newLayer)
{
	m_baseLayer = newLayer;
}
void Application::Run()
{
	//if layer is avaliable to run
	if (m_baseLayer)
	{
		//create and start later
		m_baseLayer->CreateLayer(glm::vec<2, int>(1920, 1080), "Tetra");
		m_baseLayer->Start();

		//used for dt
		float elapsedTime = 0.f;
		
		//while glfw window open
		while (!glfwWindowShouldClose(m_baseLayer->m_renderer.GetWindow().GetWindowPtr()))
		{
			//calculate dt
			float 
				glTime = static_cast<float>(glfwGetTime()),
				dt = glTime - elapsedTime;
			elapsedTime = glTime;

			//update layer
			m_baseLayer->BaseUpdate(dt);
			//render layer
			m_baseLayer->m_renderer.StartRendering();
			m_baseLayer ->BaseRender();
			m_baseLayer->m_renderer.EndRendering();
		}
		//close base layer
		m_baseLayer->End();
		//delete dynamic memory on layer
		m_baseLayer->DestroyLayer();

	}
	else
	{
		std::cout << "No base layer found!\n";
	}
}

