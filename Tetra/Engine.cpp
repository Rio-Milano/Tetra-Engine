#include "Engine.h"
#include"TestEntity.h"
#include"Program_Shader_Managment.h"
#define Program_Shader_Managment Program_Shader_Managment::GetInstance()

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>




		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


void Engine::InitializeEntities()
{
	Helper_::Status_::DisplayUsefulInfo();

	m_entities.emplace_back(std::make_shared<TestEntity>());
}

void Engine::Run()
{
	m_renderer.CreateWindow(800, 600, "Tetra");
	InitGLAD();	
	m_renderer.InitRenderer();

	m_simulation.InitSimulation(m_renderer.GetWindow().GetWindowPtr());
	InitializeEntities();

	static GLFWwindow* window{ m_renderer.GetWindow().GetWindowPtr() };
	

	while (!glfwWindowShouldClose(window))
	{
		m_simulation.Update(window);

		m_renderer.StartRendering();

		m_renderer.SetProjectionViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)), Program_Shader_Managment.GetProgramID("main"), m_renderer.GetWindow().GetWindowPtr());

		for (const std::shared_ptr<Entity>& entity : m_entities)
		{
			m_simulation.UpdateEntity(*entity.get());
			entity->Render(m_renderer);
		}

		m_renderer.EndRendering();
	}
}

void Engine::InitGLAD()
{
	//load glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize glad!\n";
	else
		std::cout << "GLAD loaded successfully!\n";
}

