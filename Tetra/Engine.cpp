#include "Engine.h"
#include"TestEntity.h"

#include<glad/glad.h>//must be in this order as glad includes headders that glfw needs, glad version 3.3
#include<GLFW/glfw3.h>

/*



		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	}
	else
	{
		glfwTerminate();//close glfw
		std::cout << "Goodbye.\n";
	}


	*/

void Engine::Run()
{
	m_renderer.CreateWindow(800, 600, "Tetra");
	InitGLAD();	
	m_renderer.InitRenderer();
	m_renderer.LoadMeshes();

	m_simulation.InitSimulation(m_renderer.GetWindow().GetWindowPtr());

	static GLFWwindow* window{ m_renderer.GetWindow().GetWindowPtr() };
	
	std::shared_ptr<TestEntity> ts_ent{std::make_shared<TestEntity>()};

	while (!glfwWindowShouldClose(window))
	{
		m_simulation.Update(window);
		m_simulation.UpdateEntity(ts_ent.get());

		m_renderer.StartRendering();
		ts_ent->Render(m_renderer);
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

