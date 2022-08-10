#include "BaseLayer.h"
#include"Program_Shader_Managment.h"
#define ShaderManager ShaderManager::GetInstance()
#include"InputManager.h"
#define InputManager InputManager::GetInstance()

void BaseLayer::CreateLayer(const glm::vec<2, int> windowSize, const std::string& windowName)
{
	m_renderer.CreateWindow(windowSize.x, windowSize.y, windowName);
	InitGLAD();
	m_renderer.InitRenderer();
	InputManager.InitializeInputManager(m_renderer.GetWindow().GetWindowPtr());
	CreateShader();
	m_camera.Initialize(90.0f, static_cast<glm::vec2>(windowSize), static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), 5.0f, .1f, glm::vec3(0.0f, 0.0f, 3.0f), m_renderer.GetWindow().GetWindowPtr());
}

void BaseLayer::DestroyLayer()
{
	m_shader.Delete();
}

void BaseLayer::BaseUpdate(const float& dt)
{
	m_renderer.GetWindow().UpdateWindow();
	m_camera.Update(dt);
	UpdateShader();
	Update(dt);

}

void BaseLayer::UpdateShader()
{
	m_shader.Use();
	GLuint projection_x_view_mat4_loc = glGetUniformLocation(m_shader.GetID(), "Projection_X_View");
	glUniformMatrix4fv(projection_x_view_mat4_loc, 1, GL_FALSE, glm::value_ptr(m_camera.GetPerspectiveViewMat4()));
}

void BaseLayer::InitGLAD()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cout << "Failed to initialize glad!\n";
	else
		std::cout << "GLAD loaded successfully!\n";
}

void BaseLayer::CreateShader()
{
	m_shader.Create("Data/Shaders/vertexShader.vert", "Data/Shaders/fragmentShader.frag");
	ShaderManager.AddShader("main", m_shader);
}
